//
// Created by pointerlost on 8/29/25.
//
#pragma once
#include <iostream>
#include <cstddef>
#include "ECS/World.h"
#include "glad/glad.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/ssbo_types.h"

namespace OpenGL {
    struct RenderCommandMDI;
    struct MDIBatch;
    class  RenderContext;
    struct EditorRenderState;
    struct GameRenderState;
}
namespace ECS {
    class World;
    struct TransformComponent;
    struct CameraComponent;
    struct MaterialComponent;
    struct LightComponent;
}

namespace OpenGL {

    struct BufferInfo {
        GLuint buffer       = 0;
        std::byte* ptr      = nullptr;
        GLsizeiptr size     = 0;
        GLsizeiptr capacity = 0;
        GLbitfield flags    = 0;
        bool isMapped       = false;

        void CleanPtr();
        void CleanBuffer();
        void CleanAll();
        ~BufferInfo();

        BufferInfo() = default;
        BufferInfo(BufferInfo&& other)            noexcept;
        BufferInfo& operator=(BufferInfo&& other) noexcept;
    };

    class BufferManager {
    public:
        void Init();
        void UpdateBuffers(ECS::World* world);
        void BuildBatches(ECS::World* world);
        void DrawAllBatches();
        void UpdateGlobals();

    private:
        BufferInfo CreateSSBO(GLsizeiptr size, GLbitfield flags = 0, bool mapBuffer = true);

        void ReAllocateGPUMemory(BufferInfo& info, GLsizeiptr newSize);

        void UpdateTransformBuffer(ECS::World* world);
        void UpdateMaterialBuffer(ECS::World* world);
        void UpdateLightBuffer(ECS::World* world);
        void UpdateCameraBuffer(ECS::World* world);

        void DrawBatch(MDIBatch& batch);
        void DrawStaticBatches();
        void DrawDynamicBatches();

        // GPU-Driven Functions
        void InitGPUBuffers();
        void BuildBatchesGPU(ECS::World* world);

        static constexpr uint32_t WORKGROUP_SIZE = 64;
    };
}