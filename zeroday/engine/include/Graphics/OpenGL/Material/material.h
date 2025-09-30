//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include <glm/ext.hpp>
#include <unordered_map>
#include <memory>
#include <optional>
#include "Core/Base.h"
#include "Graphics/OpenGL/GPU_buffers.h"

namespace Zeroday {
	namespace Ecs {
		struct MaterialComponent;
	}

	struct Texture;
}

namespace Zeroday::opengl {
    enum class MaterialTextureType {
        BaseColor,
        Normal,
        Roughness,
        Displacement,
        AmbientOcclusion,
    };

    struct Material {
        glm::vec4 baseColor = glm::vec4(1.0f);
        float metallic = 0.0f;
        float roughness = 1.0f;
        glm::vec3 emissive = glm::vec3(0.0f);

        // Base textures from material definition
        std::unordered_map<MaterialTextureType, Ref<Texture>> textures;
    };

    struct MaterialInstance {
        Ref<Material> m_Base = CreateRef<Material>();

        MaterialSSBO ToGPUFormat();

        // Simple setters
        void SetBaseColor(glm::vec4 c) { m_OverrideBaseColor = c; }
        void SetMetallic(float m) { m_OverrideMetallic = m; }
        void SetRoughness(float r) { m_OverrideRoughness = r; }
        void SetEmissive(glm::vec3 e) { m_OverrideEmissive = e; }

        void SetTexture(MaterialTextureType type, Ref<Texture> tex) {
            if (tex) {
                m_OverrideTextures[type] = tex;
                m_TextureEnabled[type] = true; // Auto-enable when setting texture
            } else {
                m_OverrideTextures.erase(type);
            }
        }

        void SetTextureEnabled(MaterialTextureType type, bool enabled) {
            m_TextureEnabled[type] = enabled;
        }

        bool IsTextureEnabled(MaterialTextureType type) const {
            if (auto it = m_TextureEnabled.find(type); it != m_TextureEnabled.end()) {
                return it->second;
            }
            return true; // Default to enabled
        }

        // Getters
        glm::vec4 GetBaseColor() const {
            return m_OverrideBaseColor.value_or(m_Base->baseColor);
        }

        float GetMetallic() const {
            return m_OverrideMetallic.value_or(m_Base->metallic);
        }

        float GetRoughness() const {
            return m_OverrideRoughness.value_or(m_Base->roughness);
        }

        glm::vec3 GetEmissive() const {
            return m_OverrideEmissive.value_or(m_Base->emissive);
        }

        Ref<Texture> GetTexture(MaterialTextureType type, Ref<Texture> defaultTexture) const {
            // If texture is disabled, return default texture
            if (!IsTextureEnabled(type)) {
                return defaultTexture;
            }

            // Check overrides first, then base material
            if (const auto it = m_OverrideTextures.find(type); it != m_OverrideTextures.end()) {
                return it->second;
            }
            if (const auto it = m_Base->textures.find(type); it != m_Base->textures.end()) {
                return it->second;
            }

            return defaultTexture;
        }

        void ResetAllOverrides() {
            m_OverrideBaseColor.reset();
            m_OverrideMetallic.reset();
            m_OverrideRoughness.reset();
            m_OverrideEmissive.reset();
            m_OverrideTextures.clear();
            m_TextureEnabled.clear();
        }

        void EnableAllTextures() {
            // Enable all texture types
            for (int i = 0; i <= static_cast<int>(MaterialTextureType::AmbientOcclusion); ++i) {
                auto type = static_cast<MaterialTextureType>(i);
                m_TextureEnabled[type] = true;
            }
        }

        void DisableAllTextures() {
            // Disable all texture types
            for (int i = 0; i <= static_cast<int>(MaterialTextureType::AmbientOcclusion); ++i) {
                auto type = static_cast<MaterialTextureType>(i);
                m_TextureEnabled[type] = false;
            }
        }

    private:
        // Simple overrides - if has value, use it, otherwise use base
        std::optional<glm::vec4> m_OverrideBaseColor;
        std::optional<float> m_OverrideMetallic;
        std::optional<float> m_OverrideRoughness;
        std::optional<glm::vec3> m_OverrideEmissive;

        // Texture overrides - if exists in map, use it, otherwise use base texture
        std::unordered_map<MaterialTextureType, Ref<Texture>> m_OverrideTextures;

        // Texture enable/disable state - if disabled, use default texture
        std::unordered_map<MaterialTextureType, bool> m_TextureEnabled;
    };
}