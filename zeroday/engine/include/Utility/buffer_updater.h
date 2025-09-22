//
// Created by pointerlost on 9/1/25.
//
#pragma once
#include <vector>

#include "core/Logger.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace Zeroday { struct MaterialSSBO; }
namespace Zeroday { struct TransformSSBO; }
namespace ecs {
    struct LightComponent;
    struct MaterialComponent;
    struct TransformComponent;
}

namespace opengl {

    struct ContiguousRange {
        uint32_t start;
        uint32_t count;
    };

    class BufferUpdater {
    public:
        // Generic contiguous range finder
        static std::vector<ContiguousRange> FindContiguousRanges(const std::vector<uint32_t>& indices);

        template<typename T, typename ComponentType>
        static void UpdateRange(const ContiguousRange& range,
                          T* gpuData,
                          const ecs::REGISTRY::ComponentRegistry<ComponentType>& registry) {
            // range validation
            if (range.start + range.count > registry.GetCapacity()) {
                Logger::Error("UpdateRange: Range out of bounds! " +
                             std::to_string(range.start) + "+" +
                             std::to_string(range.count) + " > " +
                             std::to_string(registry.GetCapacity()));
                return;
            }

            for (uint32_t i = 0; i < range.count; ++i) {
                uint32_t actualIndex = range.start + i;

                // skip free slots
                if (!registry.GetByIndex(actualIndex)) {
                    continue;
                }

                UpdateSingle<T, ComponentType>(actualIndex, gpuData, registry);
            }
        }

        // Generic single update
        template<typename T, typename ComponentType>
        static void UpdateSingle(uint32_t slotIdx,
                                T* gpuData,
                                const ecs::REGISTRY::ComponentRegistry<ComponentType>& registry) {
            // Buffer bounds check
            if (slotIdx >= registry.GetCapacity()) {
                Logger::Error("BufferUpdater: slotIdx out of bounds! " +
                             std::to_string(slotIdx) + " >= " +
                             std::to_string(registry.GetCapacity()));
                return;
            }

            if (!gpuData) {
                Logger::Error("BufferUpdater: gpuData is null!");
                return;
            }

            const auto* component = registry.GetByIndex(slotIdx);
            if (!component) return;

            // Component-specific update logic
            if constexpr (std::is_same_v<ComponentType, ecs::TransformComponent>) {
                UpdateTransformComponent(component, gpuData[slotIdx]);
            } else if constexpr (std::is_same_v<ComponentType, ecs::MaterialComponent>) {
                UpdateMaterialComponent(component,  gpuData[slotIdx]);
            } else if constexpr (std::is_same_v<ComponentType, ecs::LightComponent>) {
                UpdateLightComponent(component,     gpuData[slotIdx]);
            } else if constexpr (std::is_same_v<ComponentType, ecs::CameraComponent>) {
                UpdateCameraComponent(component,    gpuData[slotIdx]);
            }
        }

        template<typename T>
        [[nodiscard]] static bool EnsureBufferCapacity(BufferInfo& buffer,
            const ecs::REGISTRY::ComponentRegistry<T>& registry,
            size_t gpuElementSize, size_t bufferOffset = 0)
        {
            GLsizeiptr requiredSize = bufferOffset + (registry.GetCapacity() * gpuElementSize);
            if (requiredSize > buffer.capacity) {
                Logger::Warn("Buffer capacity exceeded! Required: " +
                            std::to_string(requiredSize) + ", Current: " +
                            std::to_string(buffer.capacity));
                return false;
            }
            return true;
        }

    private:
        // Component-specific update implementations
        static void UpdateTransformComponent(const ecs::TransformComponent* comp, Zeroday::TransformSSBO& gpuData);
        static void UpdateMaterialComponent(const ecs::MaterialComponent* comp,   Zeroday::MaterialSSBO&  gpuData);
        static void UpdateLightComponent(const ecs::LightComponent* comp,         Zeroday::LightSSBO&     gpuData);
        static void UpdateCameraComponent(const ecs::CameraComponent* comp,       Zeroday::CameraSSBO&    gpuData);

        static Zeroday::MaterialSSBO MakeGPUMaterialInstance(const Ref<Zeroday::MaterialInstance> &inst);
    };

}
