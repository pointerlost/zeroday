//
// Created by pointerlost on 8/29/25.
//
#include "Graphics/OpenGL/BufferManager.h"
#include "Scene/World.h"
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"
#include "Editor/State/EditorState.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Utility/buffer_updater.h"

namespace opengl {

    // ==================== BufferInfo Implementation ====================
    void BufferInfo::Clean() {
        if (ptr) {
            glUnmapNamedBuffer(buffer);
            ptr = nullptr;
        }
        if (buffer) {
            glDeleteBuffers(1, &buffer);
            buffer = 0;
        }
        size = 0;
        capacity = 0;
        isMapped = false;
    }

    BufferInfo::~BufferInfo() { Clean(); }

    BufferInfo::BufferInfo(BufferInfo&& other) noexcept {
        buffer = other.buffer;
        ptr = other.ptr;
        size = other.size;
        capacity = other.capacity;
        isMapped = other.isMapped;

        other.buffer = 0;
        other.ptr = nullptr;
        other.size = 0;
        other.capacity = 0;
        other.isMapped = false;
    }

    BufferInfo& BufferInfo::operator=(BufferInfo&& other) noexcept {
        if (this != &other) {
            Clean();
            buffer = other.buffer;
            ptr = other.ptr;
            size = other.size;
            capacity = other.capacity;
            isMapped = other.isMapped;

            other.buffer = 0;
            other.ptr = nullptr;
            other.size = 0;
            other.capacity = 0;
            other.isMapped = false;
        }
        return *this;
    }

    // ==================== BufferManager Implementation ====================
    void BufferManager::Init() {
        // Create main buffers
        transformBuffer = CreateBuffer(
            MAX_ENTITIES * sizeof(Zeroday::TransformSSBO),
            GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT,
            true
        );

        materialBuffer = CreateBuffer(
            MAX_MATERIALS * sizeof(Zeroday::MaterialSSBO),
            GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT,
            true
        );

        lightBuffer = CreateBuffer(
            MAX_LIGHTS * sizeof(Zeroday::LightSSBO),
            GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT,
            true
        );

        globalBuffer = CreateBuffer(
            sizeof(Zeroday::GlobalSSBO),
            GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT,
            true
        );

        // GPU-driven buffers
        entityMetadataBuffer = CreateBuffer(
            MAX_ENTITIES * sizeof(ecs::GPUEntityMetadata),
            GL_DYNAMIC_STORAGE_BIT,
            false
        );

        visibleEntitiesBuffer = CreateBuffer(
            MAX_ENTITIES * sizeof(uint32_t),
            GL_DYNAMIC_STORAGE_BIT,
            false
        );

        indirectCommandsBuffer = CreateBuffer(
            MAX_ENTITIES * sizeof(DrawElementsIndirectCommand),
            GL_DYNAMIC_STORAGE_BIT,
            false
        );

        // TODO: Load compute shaders here
        // transformUpdateProgram = LoadComputeShader("transform_update.comp");
        // materialUpdateProgram = LoadComputeShader("material_update.comp");
        // frustumCullProgram = LoadComputeShader("frustum_cull.comp");
        // batchBuilderProgram = LoadComputeShader("batch_builder.comp");
    }

    BufferInfo BufferManager::CreateBuffer(GLsizeiptr size, GLbitfield flags, bool mapBuffer) {
        BufferInfo info;
        info.size = size;
        info.capacity = size;

        glCreateBuffers(1, &info.buffer);
        glNamedBufferStorage(info.buffer, size, nullptr, flags);

        if (mapBuffer) {
            GLbitfield mapFlags = GL_MAP_WRITE_BIT;
            info.ptr = static_cast<std::byte*>(glMapNamedBufferRange(info.buffer, 0, size, mapFlags));
            info.isMapped = true;
        }

        return info;
    }

    void BufferManager::Update(ecs::World* world) {
        UpdateTransforms(world);
        UpdateMaterials(world);
        UpdateLights(world);
        UpdateCamera(world);
        UpdateGlobals();

        FrustumCull(world);
        BuildBatches(world);
    }

    void BufferManager::UpdateTransforms(ecs::World* world) {
        auto& transformRegistry = world->GetRegistry<ecs::TransformComponent>();
        auto& dirtyEntities = transformRegistry.GetDirtyEntities();

        if (dirtyEntities.empty()) return;

        // Update CPU-side transform data
        for (auto entity : dirtyEntities) {
            auto* transform = transformRegistry.GetComponent(entity);
            if (transform) {
                size_t index = entity.index;
                if (index < MAX_ENTITIES) {
                    Zeroday::TransformSSBO* transformData =
                        reinterpret_cast<Zeroday::TransformSSBO*>(transformBuffer.ptr + index * sizeof(Zeroday::TransformSSBO));

                    // Update transform data
                    transformData->worldMatrix = transform->GetWorldMatrix();
                    transformData->normalMatrix = glm::transpose(glm::inverse(glm::mat3(transformData->worldMatrix)));
                }
            }
        }

        transformRegistry.ClearDirty();
    }

    void BufferManager::UpdateMaterials(ecs::World* world) {
        auto& materialRegistry = world->GetRegistry<ecs::MaterialComponent>();
        auto& dirtyEntities = materialRegistry.GetDirtyEntities();

        if (dirtyEntities.empty()) return;

        for (auto entity : dirtyEntities) {
            auto* material = materialRegistry.GetComponent(entity);
            if (material) {
                size_t index = entity.index;
                if (index < MAX_MATERIALS) {
                    Zeroday::MaterialSSBO* materialData =
                        reinterpret_cast<Zeroday::MaterialSSBO*>(materialBuffer.ptr + index * sizeof(Zeroday::MaterialSSBO));

                    // Update material data
                    materialData->albedo = material->albedo;
                    materialData->metallic = material->metallic;
                    materialData->roughness = material->roughness;
                    // ... other material properties
                }
            }
        }

        materialRegistry.ClearDirty();
    }

    void BufferManager::UpdateLights(ecs::World* world) {
        auto& lightRegistry = world->GetRegistry<ecs::LightComponent>();
        auto entities = lightRegistry.GetEntities();

        // Simple light update - replace with dirty tracking if needed
        size_t lightIndex = 0;
        for (auto entity : entities) {
            if (lightIndex >= MAX_LIGHTS) break;

            auto* light = lightRegistry.GetComponent(entity);
            if (light) {
                Zeroday::LightSSBO* lightData =
                    reinterpret_cast<Zeroday::LightSSBO*>(lightBuffer.ptr + lightIndex * sizeof(Zeroday::LightSSBO));

                lightData->position = light->position;
                lightData->color = light->color;
                lightData->intensity = light->intensity;
                lightData->type = static_cast<uint32_t>(light->type);

                lightIndex++;
            }
        }
    }

    void BufferManager::UpdateCamera(ecs::World* world) {
        // Camera update logic here
    }

    void BufferManager::UpdateGlobals() {
        auto* renderContext = Services::GetRenderContext();
        auto* editorState = Services::GetEditorState();

        if (!renderContext || !editorState) return;

        Zeroday::GlobalSSBO* globalData = reinterpret_cast<Zeroday::GlobalSSBO*>(globalBuffer.ptr);
        globalData->cameraPos = editorState->cameraPosition;
        globalData->globalAmbient = glm::vec3(0.1f);
        globalData->activeLightCount = world->GetRegistry<ecs::LightComponent>().GetCount();
    }

    void BufferManager::FrustumCull(ecs::World* world) {
        // Simple frustum culling implementation
        // TODO: Replace with compute shader
    }

    void BufferManager::BuildBatches(ecs::World* world) {
        // Simple batch building
        // TODO: Replace with compute shader
    }

    void BufferManager::Draw() {
        // Bind buffers
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, transformBuffer.buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, materialBuffer.buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, lightBuffer.buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, globalBuffer.buffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, entityMetadataBuffer.buffer);

        // Draw using indirect commands
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirectCommandsBuffer.buffer);

        // Get draw count from visible entities buffer
        uint32_t drawCount = 0;
        glGetNamedBufferSubData(visibleEntitiesBuffer.buffer,
                              MAX_ENTITIES * sizeof(uint32_t),
                              sizeof(uint32_t), &drawCount);

        if (drawCount > 0) {
            glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, drawCount, 0);
        }

        // Cleanup
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
        for (int i = 0; i < 5; ++i) {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, 0);
        }
    }

    void BufferManager::ReallocateBuffer(BufferInfo& info, GLsizeiptr newSize) {
        if (newSize <= info.capacity) return;

        bool wasMapped = info.isMapped;
        info.Clean();
        info = CreateBuffer(newSize, GL_DYNAMIC_STORAGE_BIT | (wasMapped ? GL_MAP_WRITE_BIT : 0), wasMapped);
    }

}

// namespace opengl {
//
//     // ==================== BufferInfo Implementation ====================
//     void BufferInfo::CleanPtr() {
//         if (ptr) glUnmapNamedBuffer(buffer);
//         ptr = nullptr;
//     }
//
//     void BufferInfo::CleanBuffer() {
//         if (buffer) glDeleteBuffers(1, &buffer);
//         buffer = 0;
//     }
//
//     void BufferInfo::CleanAll() { CleanPtr(); CleanBuffer(); }
//     BufferInfo::~BufferInfo()   { CleanAll(); }
//
//     BufferInfo::BufferInfo(BufferInfo&& other) noexcept {
//         buffer   = other.buffer;
//         ptr      = other.ptr;
//         size     = other.size;
//         capacity = other.capacity;
//         flags    = other.flags;
//         isMapped = other.isMapped;
//         other.buffer    = 0;
//         other.ptr       = nullptr;
//         other.size      = 0;
//         other.capacity  = 0;
//         other.flags     = 0;
//         other.isMapped  = false;
//     }
//
//     BufferInfo& BufferInfo::operator=(BufferInfo&& other) noexcept {
//         if (this != &other) {
//             CleanAll();
//             buffer   = other.buffer;
//             ptr      = other.ptr;
//             size     = other.size;
//             capacity = other.capacity;
//             flags    = other.flags;
//             isMapped = other.isMapped;
//             other.buffer    = 0;
//             other.ptr       = nullptr;
//             other.size      = 0;
//             other.capacity  = 0;
//             other.flags     = 0;
//             other.isMapped  = false;
//         }
//         return *this;
//     }
//
//     // ==================== BufferManager Implementation ====================
//     void BufferManager::Init() {
//         auto& ers = Services::GetRenderContext()->GetEditorRenderState();
//
//         ers.transformBuffer = CreateSSBO(
//             MAX_ENTITIES * sizeof(Graphics::TransformSSBO),
//             GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
//             GL_MAP_PERSISTENT_BIT  | GL_MAP_COHERENT_BIT,
//             true // persistent mapping
//         );
//
//         ers.materialBuffer = CreateSSBO(
//             MAX_MATERIAL * sizeof(Graphics::MaterialSSBO),
//             GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
//             GL_MAP_PERSISTENT_BIT  | GL_MAP_COHERENT_BIT,
//             true // persistent mapping
//         );
//
//         ers.lightBuffer = CreateSSBO(
//             MAX_LIGHT * sizeof(Graphics::LightSSBO),
//             GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
//             GL_MAP_PERSISTENT_BIT  | GL_MAP_COHERENT_BIT,
//             true // persistent mapping
//         );
//
//         ers.globalBuffer = CreateSSBO(
//             1 * sizeof(Graphics::GlobalSSBO),
//             GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT |
//             GL_MAP_PERSISTENT_BIT  | GL_MAP_COHERENT_BIT,
//             true // persistent mapping
//         );
//
//         // Initialize GPU-Driven Components
//         InitComputeShaders();
//         InitGPUBuffers();
//     }
//
//     BufferInfo BufferManager::CreateSSBO(GLsizeiptr size, GLbitfield flags, bool mapBuffer) {
//         BufferInfo info{};
//         info.size = size;
//         info.capacity = size;
//
//         glCreateBuffers(1, &info.buffer);
//         if (info.buffer == 0) {
//             throw std::runtime_error("Failed to create buffer");
//         }
//
//         GLbitfield storageFlags = flags;
//         if (mapBuffer) {
//             storageFlags |= GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//         }
//
//         glNamedBufferStorage(info.buffer, size, nullptr, storageFlags);
//
//         if (mapBuffer) {
//             GLbitfield mapFlags = GL_MAP_WRITE_BIT;
//             if (storageFlags & GL_MAP_PERSISTENT_BIT) {
//                 mapFlags |= GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
//             }
//
//             info.ptr = static_cast<std::byte*>(
//                 glMapNamedBufferRange(info.buffer, 0, size, mapFlags)
//             );
//
//             if (!info.ptr) {
//                 glDeleteBuffers(1, &info.buffer);
//                 throw std::runtime_error("Failed to map buffer");
//             }
//
//             info.isMapped = true;
//         }
//
//         info.flags = storageFlags;
//         return info;
//     }
//
//     void BufferManager::InitGPUBuffers() {
//         // Entity Metadata Buffer
//         glCreateBuffers(1, &gpuEntityMetadataBuffer);
//         glNamedBufferStorage(gpuEntityMetadataBuffer, MAX_ENTITIES * sizeof(ecs::GPUEntityMetadata),
//                            nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//         // Dirty Flags Buffer
//         glCreateBuffers(1, &gpuDirtyFlagsBuffer);
//         glNamedBufferStorage(gpuDirtyFlagsBuffer, MAX_ENTITIES * sizeof(uint32_t),
//                            nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//         // Visible Entities Buffer
//         glCreateBuffers(1, &gpuVisibleEntitiesBuffer);
//         glNamedBufferStorage(gpuVisibleEntitiesBuffer, MAX_ENTITIES * sizeof(uint32_t),
//                            nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//         // Indirect Commands Buffer
//         glCreateBuffers(1, &gpuIndirectCommandsBuffer);
//         glNamedBufferStorage(gpuIndirectCommandsBuffer, MAX_ENTITIES * sizeof(DrawElementsIndirectCommand),
//                            nullptr, GL_DYNAMIC_STORAGE_BIT);
//     }
//
//     void BufferManager::UpdateTransformBuffer(ecs::World* world) {
//
//     }
//
//     void BufferManager::UpdateMaterialBuffer(ecs::World* world) {
//         auto* context = Services::GetRenderContext();
//         auto& ers = context->GetEditorRenderState();
//         auto& matRegistry = world->GetRegistry<ecs::MaterialComponent>();
//
//         auto& dirtyIndices = matRegistry.getDirty(ecs::EntityMobility::Dynamic);
//         if (dirtyIndices.empty()) return;
//
//         // Similar pattern to transform updates
//         glNamedBufferSubData(gpuDirtyFlagsBuffer, 0,
//                            dirtyIndices.size() * sizeof(uint32_t), dirtyIndices.data());
//
//         glUseProgram(materialUpdateProgram);
//         GLuint dirtyCount = static_cast<GLuint>(dirtyIndices.size());
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gpuDirtyFlagsBuffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ers.dynamicBuffer.buffer);
//         glUniform1ui(glGetUniformLocation(materialUpdateProgram, "dirtyCount"), dirtyCount);
//
//         glDispatchCompute((dirtyCount + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE, 1, 1);
//         glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//         matRegistry.clearDirty(ecs::EntityMobility::Dynamic);
//     }
//
//     void BufferManager::BuildBatchesGPU(ecs::World* world) {
//         auto& ers = Services::GetRenderContext()->GetEditorRenderState();
//
//         // First, perform frustum culling
//         glUseProgram(frustumCullProgram);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ers.staticBuffer.buffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, gpuEntityMetadataBuffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, gpuVisibleEntitiesBuffer);
//
//         // Dispatch frustum culling
//         glDispatchCompute((MAX_ENTITIES + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE, 1, 1);
//         glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//
//         // Then build batches for visible entities
//         glUseProgram(batchBuilderProgram);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gpuEntityMetadataBuffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, gpuVisibleEntitiesBuffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, gpuIndirectCommandsBuffer);
//
//         // Get visible count from GPU
//         GLuint visibleCount = 0;
//         glGetNamedBufferSubData(gpuVisibleEntitiesBuffer, MAX_ENTITIES * sizeof(uint32_t),
//                               sizeof(GLuint), &visibleCount);
//
//         glDispatchCompute((visibleCount + WORKGROUP_SIZE - 1) / WORKGROUP_SIZE, 1, 1);
//         glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
//     }
//
//     void BufferManager::BuildBatches(ecs::World* world) {
//         BuildBatchesGPU(world);
//     }
//
//     void BufferManager::UpdateBuffers(ecs::World* world) {
//
//         UpdateTransformBuffer(world);
//         UpdateMaterialBuffer(world);
//         UpdateLightBuffer(world);
//         UpdateCameraBuffer(world);
//         UpdateGlobals();
//
//         BuildBatchesGPU(world);
//     }
//
//     void BufferManager::DrawAllBatches() {
//         auto& ers = Services::GetRenderContext()->GetEditorRenderState();
//
//         // Bind buffers
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, TRANSFORM_SSBO_IDX,   ers.transformBuffer.buffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, MATERIAL_SSBO_IDX,    ers.materialBuffer.buffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, LIGHT_SSBO_IDX,       ers.lightBuffer.buffer);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, GLOBAL_DATA_SSBO_IDX, ers.globalBuffer.buffer);
//
//         // Bind indirect commands buffer
//         glBindBuffer(GL_DRAW_INDIRECT_BUFFER, gpuIndirectCommandsBuffer);
//
//         // Multi-draw indirect
//         GLuint drawCount = 0;
//         glGetNamedBufferSubData(gpuVisibleEntitiesBuffer, MAX_ENTITIES * sizeof(uint32_t),
//                               sizeof(GLuint), &drawCount);
//
//         if (drawCount > 0) {
//             glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, drawCount, 0);
//         }
//
//         // Cleanup
//         glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
//         glBindBufferBase(GL_SHADER_STORAGE_BUFFER, PAYLOAD_SSBO_IDX, 0);
//         glBindVertexArray(0);
//     }
//
//     void BufferManager::ReAllocateGPUMemory(BufferInfo& info, GLsizeiptr newSize) {
//         if (newSize <= info.capacity) return;
//
//         const bool wasMapped = (info.ptr != nullptr);
//         info.CleanAll();
//         info = CreateSSBO(newSize, GL_DYNAMIC_STORAGE_BIT, wasMapped);
//         info.capacity = newSize;
//     }
//
//     // Placeholder implementations for light and camera updates
//     void BufferManager::UpdateLightBuffer(ecs::World* world) {
//         auto* context = Services::GetRenderContext();
//         // Similar pattern to transform updates
//     }
//
//     void BufferManager::UpdateCameraBuffer(ecs::World* world) {
//         auto* context = Services::GetRenderContext();
//         // Similar pattern to transform updates
//     }
//
//     void BufferManager::UpdateGlobals() {
//         auto& ers = Services::GetRenderContext()->GetEditorRenderState();
//         const auto* editorState     = Services::GetEditorState();
//
//         const auto* cam = editorState->world->GetComponent<ecs::CameraComponent>(editorState->cameraEntity);
//         Graphics::GlobalSSBO* globalData = ers.staticBuffer.GetGlobalDataOffset();
//
//         globalData->globalAmbient    = glm::vec3(0.1f);
//         globalData->cameraPos        = cam->GetPosition();
//         globalData->activeLightCount = editorState->world->GetRegistry<ecs::LightComponent>().GetCount();
//     }
// }