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

    struct BufferOffsets {
        GLsizeiptr transform = 0;
        GLsizeiptr material = 0;
        GLsizeiptr light = 0;
        GLsizeiptr camera = 0;
        GLsizeiptr globalData = 0;
    };

    struct BufferSizes {
        size_t staticTransform = 100000;
        size_t staticMaterial = 100000;
        size_t staticLight = 500;
        size_t staticCamera = 20;
        size_t staticGlobalData = 1;

        size_t dynamicTransform = 10000;
        size_t dynamicMaterial = 10000;
        size_t dynamicLight = 50;
        size_t dynamicCamera = 12;
        size_t dynamicGlobalData = 1;
    };

    struct BufferInfo {
        GLuint buffer = 0;
        std::byte* ptr = nullptr;
        GLsizeiptr size = 0;
        GLsizeiptr capacity = 0;
        BufferOffsets offsets{};

        void CleanPtr();
        void CleanBuffer();
        void CleanAll();
        ~BufferInfo();

        BufferInfo(const BufferInfo& other) = delete;
        BufferInfo& operator=(const BufferInfo& other) = delete;
        BufferInfo() = default;
        BufferInfo(BufferInfo&& other) noexcept;
        BufferInfo& operator=(BufferInfo&& other) noexcept;

        template <typename T> const T* as(GLsizeiptr offset) const;
        template <typename T> T* as(GLsizeiptr offset);

        Graphics::TransformSSBO* GetTransformOffset();
        Graphics::MaterialSSBO* GetMaterialOffset();
        Graphics::LightSSBO* GetLightOffset();
        Graphics::CameraSSBO* GetCameraOffset();
        Graphics::GlobalFragmentSSBO* GetGlobalDataOffset();

        const Graphics::TransformSSBO* GetTransformOffset() const;
        const Graphics::MaterialSSBO* GetMaterialOffset() const;
        const Graphics::LightSSBO* GetLightOffset() const;
        const Graphics::CameraSSBO* GetCameraOffset() const;
        const Graphics::GlobalFragmentSSBO* GetGlobalDataOffset() const;
    };

    class BufferManager {
    public:
        BufferManager();
        void Init();
        void UpdateBuffers(ECS::World* world);
        void TakeGameSnapshotFromEditorState();
        void BuildBatches(ECS::World* world);
        void DrawAllBatches();
        void UpdateGlobals();

    private:
        BufferInfo CreateSSBO(GLsizeiptr size, GLbitfield flags = 0, bool mapBuffer = true);
        BufferSizes bufferSizes;

        void ReAllocateGPUMemory(BufferInfo& info, GLsizeiptr newSize);
        void SnapshotBuffer(const BufferInfo& src, BufferInfo& dst);

        BufferInfo& GetBufferForMobility(EditorRenderState& ers, ECS::EntityMobility mobility);
        const BufferInfo& GetBufferForMobility(const EditorRenderState& ers, ECS::EntityMobility mobility) const;

        void UpdateTransformsGPU(RenderContext* context, ECS::World* world);
        void UpdateMaterialsGPU(RenderContext* context, ECS::World* world);
        void UpdateLightsGPU(RenderContext* context, ECS::World* world);
        void UpdateCameraGPU(RenderContext* context, ECS::World* world);

        void DrawBatch(MDIBatch& batch);
        void DrawStaticBatches();
        void DrawDynamicBatches();

        // GPU-Driven Functions
        void InitComputeShaders();
        void InitGPUBuffers();
        void BuildBatchesGPU(ECS::World* world);
        GLuint CreatePersistentBuffer(GLsizeiptr size);

        // Compute Shaders
        GLuint transformUpdateProgram = 0;
        GLuint materialUpdateProgram = 0;
        GLuint lightUpdateProgram = 0;
        GLuint cameraUpdateProgram = 0;
        GLuint batchBuilderProgram = 0;
        GLuint frustumCullProgram = 0;

        // GPU Buffers
        GLuint gpuEntityMetadataBuffer = 0;
        GLuint gpuDirtyFlagsBuffer = 0;
        GLuint gpuVisibleEntitiesBuffer = 0;
        GLuint gpuIndirectCommandsBuffer = 0;

        static constexpr uint32_t WORKGROUP_SIZE = 64;
    };
}