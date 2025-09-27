//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include <glm/ext.hpp>
#include <unordered_map>
#include <memory>
#include <optional>
#include "core/Base.h"
#include "Graphics/OpenGL/GPU_buffers.h"

namespace Zeroday {
	namespace Ecs {
		struct MaterialComponent;
	}

	struct Texture;}

namespace Zeroday::opengl {

	enum class MaterialTextureType {
		BaseColor,
		Metallic,
		Roughness,
		Emissive,
		Normal,
		AmbientOcclusion,
		Displacement,
		Height,
	};

	struct Material
	{
		std::string name    = {};
		glm::vec4 baseColor = glm::vec4(1.0f); // RGBA
		float     metallic  = 0.0f;
		float     roughness = 1.0f;
		glm::vec3 emissive  = glm::vec3(0.0f);

		std::unordered_map<MaterialTextureType, Ref<Texture>> textures;
	};

	struct MaterialInstance {
		Ref<Material> m_Base = CreateRef<Material>();

		MaterialSSBO ToGPUFormat();

		void SetBaseColor(glm::vec4 c) { m_OverrideBaseColor    = c; }
		void SetMetallic(float m)      { m_OverrideMetallic  = m; }
		void SetRoughness(float r)     { m_OverrideRoughness = r; }
		void SetEmissive(glm::vec3 e)  { m_OverrideEmissive     = e; }
		void SetTexture(MaterialTextureType type, Ref<Texture> tex) {
			m_OverrideTextures[type] = std::move(tex);
		}

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

		Ref<Texture> GetTexture(MaterialTextureType type) const {
			if (const auto it = m_OverrideTextures.find(type); it != m_OverrideTextures.end())
				return it->second;
			if (const auto it = m_Base->textures.find(type); it != m_Base->textures.end())
				return it->second;
			return nullptr;
		}

		void ResetOverrides() {
			m_OverrideBaseColor.reset();
			m_OverrideMetallic.reset();
			m_OverrideRoughness.reset();
			m_OverrideEmissive.reset();
			m_OverrideTextures.clear();
		}

	private:
		std::optional<glm::vec4> m_OverrideBaseColor;
		std::optional<float>     m_OverrideMetallic;
		std::optional<float>     m_OverrideRoughness;
		std::optional<glm::vec3> m_OverrideEmissive;
		std::unordered_map<MaterialTextureType, Ref<Texture>> m_OverrideTextures;
	};

}