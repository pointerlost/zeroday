//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include <glm/ext.hpp>
#include <unordered_map>
#include <memory>
#include <optional>
#include "core/Base.h"

namespace Zeroday { struct Texture;}

namespace Zeroday {

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
		Ref<Material> base = CreateRef<Material>();

		void setBaseColor(glm::vec4 c) { overrideBaseColor    = c; }
		void setMetallic(float m)      { overrideMetallic  = m; }
		void setRoughness(float r)     { overrideRoughness = r; }
		void setEmissive(glm::vec3 e)  { overrideEmissive     = e; }
		void setTexture(MaterialTextureType type, Ref<Texture> tex) {
			overrideTextures[type] = std::move(tex);
		}

		glm::vec4 getBaseColor() const {
			return overrideBaseColor.value_or(base->baseColor);
		}
		float getMetallic() const {
			return overrideMetallic.value_or(base->metallic);
		}
		float getRoughness() const {
			return overrideRoughness.value_or(base->roughness);
		}
		glm::vec3 getEmissive() const {
			return overrideEmissive.value_or(base->emissive);
		}

		Ref<Texture> getTexture(MaterialTextureType type) const {
			if (const auto it = overrideTextures.find(type); it != overrideTextures.end())
				return it->second;
			if (const auto it = base->textures.find(type); it != base->textures.end())
				return it->second;
			return nullptr;
		}

		void resetOverrides() {
			overrideBaseColor.reset();
			overrideMetallic.reset();
			overrideRoughness.reset();
			overrideEmissive.reset();
			overrideTextures.clear();
		}

	private:
		std::optional<glm::vec4> overrideBaseColor;
		std::optional<float>     overrideMetallic;
		std::optional<float>     overrideRoughness;
		std::optional<glm::vec3> overrideEmissive;
		std::unordered_map<MaterialTextureType, Ref<Texture>> overrideTextures;
	};

}