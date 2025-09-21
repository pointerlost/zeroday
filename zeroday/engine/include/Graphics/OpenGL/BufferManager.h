//
// Created by pointerlost on 8/29/25.
//
#pragma once
#include <iostream>
#include <cstddef>
#include "Scene/World.h"
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
    class World;
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
        void Update(ecs::World* world);
        void Draw();

    private:
        BufferInfo CreateBuffer(GLsizeiptr size, GLbitfield flags, bool mapBuffer);
        void ReallocateBuffer(BufferInfo& info, GLsizeiptr newSize);

        void UpdateTransforms(ecs::World* world);
        void UpdateMaterials(ecs::World* world);
        void UpdateLights(ecs::World* world);
        void UpdateCamera(ecs::World* world);
        void UpdateGlobals();

        void BuildBatches(ecs::World* world);

        // Buffers
        BufferInfo transformBuffer;
        BufferInfo materialBuffer;
        BufferInfo lightBuffer;
        BufferInfo globalBuffer;
        BufferInfo entityMetadataBuffer;
        BufferInfo visibleEntitiesBuffer;
        BufferInfo indirectCommandsBuffer;

        // Shader programs
        GLuint transformUpdateProgram = 0;
        GLuint materialUpdateProgram = 0;
        GLuint frustumCullProgram = 0;
        GLuint batchBuilderProgram = 0;

        static constexpr uint32_t WORKGROUP_SIZE = 64;
        static constexpr uint32_t MAX_ENTITIES = 10000;
        static constexpr uint32_t MAX_MATERIALS = 1000;
        static constexpr uint32_t MAX_LIGHTS = 100;
    };
}