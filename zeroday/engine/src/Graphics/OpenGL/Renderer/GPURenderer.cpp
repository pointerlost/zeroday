//
// Created by pointerlost on 8/29/25.
//
#include "Graphics/OpenGL/Renderer/GPURenderer.h"
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "core/AssetManager.h"
#include "core/Logger.h"
#include "core/Services.h"

namespace Zeroday::opengl {

    template<typename T, BufferType Type>
    void* GPUBuffer<T, Type>::BeginUpdate() {
        if (!m_MappedPtr) {
            // Set capacity before creating buffer
            if (m_Capacity == 0) {
                m_Capacity = 1024 * sizeof(T);  // default
            }

            glCreateBuffers(1, &m_Buffer);
            glNamedBufferStorage(m_Buffer, m_Capacity, nullptr, m_Flags);
            m_MappedPtr = glMapNamedBufferRange(m_Buffer, 0, m_Capacity,
                                              GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        }
        return m_MappedPtr;
    }

    template <typename T, BufferType Type>
    void GPUBuffer<T, Type>::Upload(const std::vector<T>& data) {
        m_CPUDatas = data;
        size_t newSize = data.size() * sizeof(T);

        if (m_MappedPtr) {
            // Use persistent mapping
            if (data.size() * sizeof(T) > m_Capacity) {
                // Reallocate
                glUnmapNamedBuffer(m_Buffer);
                glDeleteBuffers(1, &m_Buffer);
                m_Capacity = data.size() * sizeof(T);
                glCreateBuffers(1, &m_Buffer);
                glNamedBufferStorage(m_Buffer, m_Capacity, nullptr, m_Flags);
                m_MappedPtr = glMapNamedBufferRange(m_Buffer, 0, m_Capacity,
                                                  GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
            }
            memcpy(m_MappedPtr, data.data(), data.size() * sizeof(T));
            glFlushMappedNamedBufferRange(m_Buffer, 0, data.size() * sizeof(T));
        } else {
            // Non-persistent mapping
            if (m_Buffer == 0) {
                glCreateBuffers(1, &m_Buffer);

                if constexpr (Type == BufferType::UBO) {
                    // UBOs have different usage patterns
                    glNamedBufferStorage(m_Buffer, newSize, data.data(),
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);
                } else {
                    // SSBO/TBO default behavior
                    glNamedBufferStorage(m_Buffer, newSize, data.data(), m_Flags);
                }

                m_Capacity = newSize;
            }
            else if (newSize > m_Capacity) {
                // Reallocate if bigger
                glDeleteBuffers(1, &m_Buffer);
                glCreateBuffers(1, &m_Buffer);

                if constexpr (Type == BufferType::UBO) {
                    glNamedBufferStorage(m_Buffer, newSize, data.data(),
                        GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT);  // UBO flags
                } else {
                    glNamedBufferStorage(m_Buffer, newSize, data.data(), m_Flags); // SSBO flags
                }

                m_Capacity = newSize;
            }
            else {
                // Update existing
                glNamedBufferSubData(m_Buffer, 0, newSize, data.data());
            }
        }
    }

    template <typename T, BufferType Type>
    void GPUBuffer<T, Type>::Upload(const T& single) {
        Upload(std::vector<T>{ single });
    }

    // Specialize Bind for different buffer types
    template<typename T, BufferType Type>
    void GPUBuffer<T, Type>::Bind(GLuint bindingPoint) const {
        if constexpr (Type == BufferType::UBO) {
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Buffer);
        } else if constexpr (Type == BufferType::SSBO) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, m_Buffer);
        }
    }

    void GPURenderer::Init() {
        // Load shaders
        if (!Services::GetAssetManager()->LoadAllShaders()) {
            Error("[Engine::initResources] LoadAllShaders FAILED!");
        }

        // Pre-allocate buffers with persistent mapping
        m_TransformBuffer.Upload(std::vector<TransformSSBO>(MAX_ENTITIES));
        m_MaterialBuffer.Upload(std::vector<MaterialSSBO>(MAX_ENTITIES));
        m_LightBuffer.Upload(std::vector<LightSSBO>(MAX_LIGHTS));
        m_IndirectCommandBuffer.Upload(std::vector<DrawElementsIndirectCommand>(MAX_ENTITIES));
        m_PayloadBuffer.Upload(std::vector<DrawPayloadGPU>(MAX_ENTITIES));
    }

    void GPURenderer::Render() {
        if (!m_Scene) return;
        // Extract + Upload (CPU -> GPU)
        CollectSceneData();
        BindBuffers();
        RenderFrame();
    }

    void GPURenderer::CollectSceneData() {
        auto extracted = SceneRenderer::ExtractRenderables(m_Scene);

        // Use persistent mapping for frequent updates
        {
            void* transformData = m_TransformBuffer.BeginUpdate();
            memcpy(transformData, extracted.transforms.data(), extracted.transforms.size() * sizeof(TransformSSBO));
            m_TransformBuffer.EndUpdate(extracted.transforms.size() * sizeof(TransformSSBO));
        }

        {
            void* materialData = m_MaterialBuffer.BeginUpdate();
            memcpy(materialData, extracted.materials.data(), extracted.materials.size() * sizeof(MaterialSSBO));
            m_MaterialBuffer.EndUpdate(extracted.materials.size() * sizeof(MaterialSSBO));
        }

        // Generate commands/payloads directly into mapped memory
        {
            auto* commands = static_cast<DrawElementsIndirectCommand*>(m_IndirectCommandBuffer.BeginUpdate());
            auto* payloads = static_cast<DrawPayloadGPU*>(m_PayloadBuffer.BeginUpdate());

            for (uint i = 0; i < extracted.renderCommands.size(); i++) {
                auto& cmd = extracted.renderCommands[i];
                commands[i] = {cmd.indexCount, 1, cmd.indexOffset, 0, i};
                payloads[i] = {(int)cmd.transformIndex, (int)cmd.materialIndex, (int)cmd.vao, (int)i};
            }

            m_IndirectCommandBuffer.EndUpdate(extracted.renderCommands.size() * sizeof(DrawElementsIndirectCommand));
            m_PayloadBuffer.EndUpdate(extracted.renderCommands.size() * sizeof(DrawPayloadGPU));
        }

        // sync before GPU access
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // UBOs
        GlobalUBO global{glm::vec3(0.1f)};
        m_GlobalBuffer.Upload(global);
        m_CameraBuffer.Upload(extracted.camera);
    }

    void GPURenderer::BindBuffers() {
        m_RenderCommandsOfPerEntity.Bind(0);
        m_IndirectCommandBuffer.Bind(1);
        m_PayloadBuffer.Bind(2);
        m_TransformBuffer.Bind(3);
        m_MaterialBuffer.Bind(4);
        m_LightBuffer.Bind(5);
        m_GlobalBuffer.Bind(6);
        m_CameraBuffer.Bind(7);
    }

    void GPURenderer::RenderFrame() {
        AssetManager::GetShader("main")->bind();
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_IndirectCommandBuffer.GetHandle());
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr,
                                   m_RenderCommandsOfPerEntity.GetCount(), 0);
    }
}
