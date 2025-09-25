//
// Created by pointerlost on 8/29/25.
//
#pragma once
#include "glad/glad.h"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/Renderer/RenderCommand.h"

namespace Zeroday { class Camera; }

namespace Zeroday::opengl {
    struct GlobalUBO;
    struct LightSSBO;
    struct MaterialSSBO;
    struct TransformSSBO;
    struct CameraUBO;
}

namespace Zeroday::opengl {

    enum class BufferType {
        SSBO, // Shader Storage Buffer Object
        UBO,  // Uniform Buffer Object
        // TBO(texture Buffer Object), to be added (later)
    };

    template <typename T, BufferType Type = BufferType::SSBO>
    class GPUBuffer {
    public:
        explicit GPUBuffer(GLbitfield flags = GL_DYNAMIC_STORAGE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT)
        : m_Flags(flags) {}

        ~GPUBuffer() {
            if (m_MappedPtr) {
                glUnmapNamedBuffer(m_Buffer);
            }
        }

        void* BeginUpdate();

        void EndUpdate(size_t dataSize) {
            if (m_MappedPtr) {
                glFlushMappedNamedBufferRange(m_Buffer, 0, dataSize);
            }
        }

        void Upload(const std::vector<T>& data);
        void Upload(const T& single);
        void Bind(GLuint bindingPoint) const;

        [[nodiscard]] GLuint GetHandle()   const { return m_Buffer; }
        [[nodiscard]] size_t GetCount()    const { return m_CPUDatas.size(); }
        [[nodiscard]] size_t GetCapacity() const { return m_Capacity; }

    private:
        void* m_MappedPtr = nullptr;
        GLuint m_Buffer = 0;
        std::vector<T> m_CPUDatas;
        size_t m_Capacity = 0;
        GLbitfield m_Flags = GL_DYNAMIC_STORAGE_BIT;
    };

    class GPURenderer {
    public:
        explicit GPURenderer(Scene* scene) : m_Scene(scene) {}

        void Init();
        void Render();
        void Shutdown();

    private:
        void CollectSceneData();
        void BindBuffers();
        void RenderFrame();

        Scene* m_Scene = nullptr;

        // Template-based buffers
        GPUBuffer<TransformSSBO, BufferType::SSBO> m_TransformBuffer;
        GPUBuffer<MaterialSSBO, BufferType::SSBO> m_MaterialBuffer;
        GPUBuffer<LightSSBO, BufferType::SSBO> m_LightBuffer;
        GPUBuffer<RenderCommandMDI, BufferType::SSBO> m_RenderCommandsOfPerEntity;
        GPUBuffer<DrawElementsIndirectCommand, BufferType::SSBO> m_IndirectCommandBuffer;
        GPUBuffer<DrawPayloadGPU, BufferType::SSBO> m_PayloadBuffer;

        GPUBuffer<GlobalUBO, BufferType::UBO> m_GlobalBuffer;
        GPUBuffer<CameraUBO, BufferType::UBO> m_CameraBuffer;

        uint32_t m_CurrentLightCount = 0;
    };

}