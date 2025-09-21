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
		[[nodiscard]] bool createMaterials(const std::string& filePath);

		[[nodiscard]] std::shared_ptr<MaterialInstance> createInstance(const std::string& name);

		[[nodiscard]] std::shared_ptr<Material> getMaterialByName(const std::string& name);

		[[nodiscard]] std::shared_ptr<Material> getDefaultMaterial() const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
	};
}