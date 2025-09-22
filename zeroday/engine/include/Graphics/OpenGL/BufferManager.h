//
// Created by pointerlost on 8/29/25.
//
#pragma once
#include <iostream>
#include <cstddef>
#include "Scene/Scene.h"
#include "glad/glad.h"

namespace Zeroday {
    struct LightSSBO;
    struct TransformSSBO;
    struct MaterialSSBO;
}

namespace opengl {
    struct RenderCommandMDI;
    struct MDIBatch;
    class  RenderContext;
    struct EditorRenderState;
    struct GameRenderState;
}

namespace ecs {
    class Scene;
    struct TransformComponent;
    struct CameraComponent;
    struct MaterialComponent;
    struct LightComponent;
}

namespace opengl {

    struct BufferInfo {
        GLuint buffer = 0;
        std::byte* ptr = nullptr;
        GLsizeiptr size = 0;
        GLsizeiptr capacity = 0;
        bool isMapped = false;

        void Clean();
        ~BufferInfo();
        BufferInfo(BufferInfo&& other) noexcept;
        BufferInfo& operator=(BufferInfo&& other) noexcept;
    };

    class BufferManager {
    public:
        void Init();
        void Update(ecs::Scene* world);
        void Draw();

    private:
        BufferInfo CreateBuffer(GLsizeiptr size, GLbitfield flags, bool mapBuffer = true);
        void ReallocateBuffer(BufferInfo& info, GLsizeiptr newSize);

        void UpdateTransforms(ecs::Scene* world);
        void UpdateMaterials(ecs::Scene* world);
        void UpdateLights(ecs::Scene* world);
        void UpdateCamera(ecs::Scene* world);
        void UpdateGlobals(ecs::Scene* world);

        void BuildBatches(ecs::Scene* world);
        void DispatchCulling();
        void ExecuteDrawCommands();

        // Core buffers (aligned with your SSBO structs)
        BufferInfo transformBuffer;
        BufferInfo materialBuffer;
        BufferInfo lightBuffer;
        BufferInfo cameraBuffer;
        BufferInfo globalBuffer;

        // GPU-driven pipeline buffers
        BufferInfo entityIndexBuffer;       // Entity IDs for GPU processing
        BufferInfo visibleIndexBuffer;      // Output from culling
        BufferInfo drawCommandBuffer;       // Indirect draw commands
        BufferInfo batchDataBuffer;         // Batch metadata

        // Atomic counter for GPU synchronization
        BufferInfo atomicCounterBuffer;

        // Shader programs
        GLuint cullingComputeProgram = 0;
        GLuint batchBuilderProgram = 0;
        GLuint mainRenderProgram = 0;

        // Mesh management
        std::unordered_map<std::string, MeshData> meshLibrary;

        // Statistics (i need to update | add a Statistics struct for these stuffs)
        uint32_t visibleEntityCount = 0;
        uint32_t totalEntityCount = 0;

        static constexpr uint32_t WORKGROUP_SIZE = 64;
        static constexpr uint32_t MAX_ENTITIES = 65536; // Power of 2 for GPU efficiency
        static constexpr uint32_t MAX_LIGHTS = 256;
        static constexpr uint32_t MAX_BATCHES = 1024;
    };
}