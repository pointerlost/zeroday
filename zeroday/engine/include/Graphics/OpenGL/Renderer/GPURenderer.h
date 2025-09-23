//
// Created by pointerlost on 8/29/25.
//
#pragma once
#include <glm/ext.hpp>
#include "Scene/Scene.h"
#include "glad/glad.h"


namespace ecs {
    class Scene;
}

namespace Zeroday {
    class Camera;
}

namespace Zeroday::opengl {
    struct DrawPayloadGPU;
    struct DrawElementsIndirectCommand;
    struct RenderCommandMDI;
    struct GlobalSSBO;
    struct LightSSBO;
    struct MaterialSSBO;
    struct TransformSSBO;
    struct CameraUBO;
}

namespace Zeroday::opengl {

    template <typename T>
    class GPUBuffer {
    private:
        GLuint m_Buffer = 0;
        std::vector<T> m_CPUDatas;
        size_t m_Capacity = 0;
        GLbitfield m_Flags = GL_DYNAMIC_STORAGE_BIT;

    public:
        explicit GPUBuffer(GLbitfield flags = GL_DYNAMIC_STORAGE_BIT) : m_Flags(flags) {}
        ~GPUBuffer();

        void Upload(const std::vector<T>& data);

        [[nodiscard]] GLuint GetHandle() const { return m_Buffer;   }
        [[nodiscard]] size_t GetCount()  const { return m_Capacity; }
    };

    class GPURenderer {
    public:
        explicit GPURenderer(Scene* scene) : m_Scene(scene) {}

        void Init();
        void Render();
        void Shutdown();

    private:
        void CollectSceneData();
        void UploadToGPU();
        void DispatchComputeShaders();
        void BindBuffers();
        void RenderFrame();

        Scene* m_Scene = nullptr;

        // Template-based buffers (clean and type-safe)
        GPUBuffer<RenderCommandMDI> m_RenderCommandsOfPerEntity;
        GPUBuffer<TransformSSBO> m_CPUTransformBuffer;
        GPUBuffer<TransformSSBO> m_GPUTransformBuffer;
        GPUBuffer<MaterialSSBO> m_CPUMaterialBuffer;
        GPUBuffer<MaterialSSBO> m_GPUMaterialBuffer;
        GPUBuffer<LightSSBO> m_LightBuffer;
        GPUBuffer<GlobalSSBO> m_GlobalBuffer;
        GPUBuffer<DrawElementsIndirectCommand> m_CommandBuffer;
        GPUBuffer<DrawPayloadGPU> m_PayloadBuffer;
        GPUBuffer<CameraUBO> m_CameraBuffer;

        // Shaders
        GLuint m_ComputeShaders[5] = {0};
        GLuint m_RenderPipeline = 0;
    };

}