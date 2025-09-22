#pragma once
#include <memory>
#include <unordered_map>
#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include "material.h"

// Forward declaration
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

		[[nodiscard]] Ref<MaterialInstance> CreateInstance(const std::string& name);

		[[nodiscard]] Ref<Material> GetMaterialByName(const std::string& name);

		[[nodiscard]] Ref<Material> GetDefaultMaterial() const;

	private:
		std::unordered_map<std::string, Ref<Material>> m_materials;
	};
}