//
// Created by pointerlost on 8/29/25.
//
#include "Graphics/OpenGL/Renderer/GPURenderer.h"

#include "core/AssetManager.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"

namespace Zeroday::opengl {

    template <typename T>
    void GPUBuffer<T>::Upload(const std::vector<T>& data) {
        m_CPUDatas = data;
        size_t newSize = data.size() * sizeof(T);

        if (m_Buffer == 0) {
            // First-time creation
            glCreateBuffers(1, &m_Buffer);
            glNamedBufferStorage(m_Buffer, newSize, data.data(), m_Flags);
            m_Capacity = newSize;
        }
        else if (newSize > m_Capacity) {
            // Reallocate if bigger
            glDeleteBuffers(1, &m_Buffer);
            glCreateBuffers(1, &m_Buffer);
            glNamedBufferStorage(m_Buffer, newSize, data.data(), m_Flags);
            m_Capacity = newSize;
        }
        else {
            // Update existing
            glNamedBufferSubData(m_Buffer, 0, newSize, data.data());
        }
    }

    template<typename T>
    GPUBuffer<T>::~GPUBuffer() {
        if (m_Buffer) glDeleteBuffers(1, &m_Buffer);
    }

    void GPURenderer::Init() {
        // Compile shaders
        const char* computeShaders[] = {
            "shaders/transform_update.comp",
            "shaders/material_update.comp",
            "shaders/light_update.comp",
            "shaders/global_update.comp",
            "shaders/command_generation.comp"
        };

        for (int i = 0; i < 5; i++) {
            m_ComputeShaders[i] = AssetManager::CompileComputeShader(computeShaders[i]);
        }
    }

    void GPURenderer::Render() {
        if (!m_Scene) return;

        // 1. Extract data from ECS
        CollectSceneData();

        // 2. Upload to GPU
        UploadToGPU();

        // 3. Process with compute shaders
        DispatchComputeShaders();

        // 4. Render
        RenderFrame();
    }

    void GPURenderer::CollectSceneData() {
        auto extracted = SceneRenderer::ExtractRenderables(m_Scene);

        // Fixed: Upload glm::mat4, not TransformSSBO
        m_CPUTransformBuffer.Upload(extracted.transforms);
        m_CPUMaterialBuffer.Upload(extracted.materials);
        m_LightBuffer.Upload(extracted.lights);
        m_RenderCommandsOfPerEntity.Upload(extracted.renderCommands);

        // Upload camera array
        if (!extracted.cameras.empty()) {
            m_CameraBuffer.Upload(extracted.cameras);
        }

        // Update global data
        GlobalSSBO global;
        global.globalAmbient = glm::vec3(0.1f);
        m_GlobalBuffer.Upload({global});
    }

    void GPURenderer::UploadToGPU() {
    }

    void GPURenderer::DispatchComputeShaders() {
        // Bind buffers first
        BindBuffers();

        // Dispatch each compute shader
        const uint32_t entityCount = m_RenderCommandsOfPerEntity.GetCount();
        const uint32_t groupCount  = (entityCount + 63) / 64;

        for (int i = 0; i < 4; i++) { // data upload shaders
            glUseProgram(m_ComputeShaders[i]);
            glDispatchCompute(groupCount, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
        }

        // Command generation
        glUseProgram(m_ComputeShaders[4]);
        glUniform1ui(glGetUniformLocation(m_ComputeShaders[4], "totalEntityCount"), entityCount);
        glDispatchCompute(groupCount, 1, 1);
        glMemoryBarrier(GL_COMMAND_BARRIER_BIT);
    }

    void GPURenderer::BindBuffers() {
        // Entity metadata
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, m_RenderCommandsOfPerEntity.GetHandle());

        // CPU->GPU data (binding 0)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_CPUTransformBuffer.GetHandle());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_CPUMaterialBuffer.GetHandle());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_LightBuffer.GetHandle());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_GlobalBuffer.GetHandle());

        // GPU-side storage (binding 1)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_GPUTransformBuffer.GetHandle());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_GPUMaterialBuffer.GetHandle());

        // Commands & payloads
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, m_CommandBuffer.GetHandle());
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 8, m_PayloadBuffer.GetHandle());
    }

    void GPURenderer::RenderFrame() {
        AssetManager::GetShader("MAIN")->bind();
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_CommandBuffer.GetHandle());
        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr,
                                   m_RenderCommandsOfPerEntity.GetCount(), 0);
    }
}
