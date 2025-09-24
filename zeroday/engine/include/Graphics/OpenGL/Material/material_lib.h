#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "material.h"

namespace Zeroday
{
	class TextureManager;
	struct Texture;
};

using json = nlohmann::json;

inline constexpr auto BASIC_SHADER = "basic";

namespace Zeroday {

	class MaterialLibrary {
	public:
		[[nodiscard]] bool CreateMaterials(const std::string& filePath);

		[[nodiscard]] Ref<opengl::MaterialInstance> CreateInstance(const std::string& name);

		[[nodiscard]] Ref<opengl::Material> GetMaterialByName(const std::string& name);

		[[nodiscard]] Ref<opengl::Material> GetDefaultMaterial() const;

	private:
		std::unordered_map<std::string, Ref<opengl::Material>> m_materials;
	};
}