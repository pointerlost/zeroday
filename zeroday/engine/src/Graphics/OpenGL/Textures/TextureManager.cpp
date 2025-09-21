//
// Created by pointerlost on 8/15/25.
//
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include <algorithm>
#include <filesystem>
#include <ranges>
#include "core/Logger.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Graphics/OpenGL/Textures/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/detail/string_concat.hpp>
#include "Graphics/OpenGL/Material/material.h"

namespace Zeroday
{
	TextureManager::TextureManager()
	{
		// Modern PBR default textures (1x1 pixel)
		m_defaultTextures[MaterialTextureType::BaseColor]        = CreateDefaultTexture("default_basecolor", 255, 255, 255 );
		m_defaultTextures[MaterialTextureType::Normal]           = CreateDefaultTexture("default_normal",    128, 128, 255 );
		m_defaultTextures[MaterialTextureType::Roughness]        = CreateDefaultTexture("default_roughness", 255, 255, 255 );
		m_defaultTextures[MaterialTextureType::Metallic]         = CreateDefaultTexture("default_metallic",    0,   0,   0 );
		m_defaultTextures[MaterialTextureType::AmbientOcclusion] = CreateDefaultTexture("default_ao",        255, 255, 255 );
		m_defaultTextures[MaterialTextureType::Emissive]         = CreateDefaultTexture("default_emissive",    0,   0,   0 );

		// add to maps
		for (auto &tex: m_defaultTextures | std::views::values) {
			if (tex) {
				m_allTextures.push_back(tex);
				m_nameMap[tex->name] = tex;
			}
		}
	}

	TextureManager::~TextureManager() {
		// Note: ensure this runs on GL thread or call releaseAll manually on shutdown
		releaseAll();
	}

	std::shared_ptr<Texture> TextureManager::load(const std::string& name, const std::string &path) {
		if (const auto it  = m_pathMap.find(path); it != m_pathMap.end()) return it->second;

		if (const auto itn = m_nameMap.find(name); itn != m_nameMap.end()) {
			m_pathMap[path] = itn->second;
			return itn->second;
		}

		int width = 0, height = 0, channels = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data) {
			Logger::Warn("Failed to load texture: " + path);
			return GetDefaultTexture(MaterialTextureType::BaseColor);
		}

		const GLenum format = (channels == 4) ? GL_RGBA : (channels == 3) ? GL_RGB : GL_RED;
		const GLint internalFormat = (channels == 4) ? GL_RGBA8 : (channels == 3) ? GL_RGB8 : GL_R8;

		GLuint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);

		auto tex = std::make_shared<Texture>();
		tex->name = name;
		tex->glID = texID;

		if (GLAD_GL_ARB_bindless_texture && GLAD_GL_ARB_gpu_shader_int64) {
			if (const uint64_t handle = glGetTextureHandleARB(texID)) {
				glMakeTextureHandleResidentARB(handle);
				tex->bindlessHandle = handle;
				tex->resident = true;
				residentHandles.insert(handle);
			}
		}

		m_nameMap[name] = tex;
		m_pathMap[path] = tex;
		m_allTextures.push_back(tex);
		return tex;
	}

	std::shared_ptr<Texture> TextureManager::getTextureWithName(const std::string &name) {
		if (!m_nameMap.contains(name)) {
			Logger::Warn("Texture \"" + name + "\" not found.");
			return nullptr;
		}
		return m_nameMap[name];
	}

	std::shared_ptr<Texture> TextureManager::getTextureWithPath(const std::string &path) {
		if (!m_pathMap.contains(path)) {
			Logger::Warn("Texture \"" + path + "\" not found.");
			return nullptr;
		}
		return m_pathMap[path];
	}

	std::shared_ptr<Texture> TextureManager::GetDefaultTexture(MaterialTextureType type) {
		if (const auto it = m_defaultTextures.find(type); it != m_defaultTextures.end())
			return it->second;
		return m_defaultTextures[MaterialTextureType::BaseColor];
	}

	uint64_t TextureManager::ensureBindlessHandle(std::shared_ptr<Texture> tex) {
		if (!tex || tex->bindlessHandle) return tex ? tex->bindlessHandle : 0;
		if (!GLAD_GL_ARB_bindless_texture || !GLAD_GL_ARB_gpu_shader_int64) return 0;
		if (!glIsTexture(tex->glID)) return 0;

		const uint64_t handle = glGetTextureHandleARB(tex->glID);
		if (!handle) return 0;

		glMakeTextureHandleResidentARB(handle);
		tex->bindlessHandle = handle;
		tex->resident = true;
		residentHandles.insert(handle);
		return handle;
	}

	uint64_t TextureManager::getBindlessHandle(std::shared_ptr<Texture> tex) const {
		return tex ? tex->bindlessHandle : 0;
	}

    void TextureManager::releaseTexture(const std::shared_ptr<Texture> &tex) {
		if (!tex) return;

		if (tex->resident && tex->bindlessHandle) {
			glMakeTextureHandleNonResidentARB(tex->bindlessHandle);
			residentHandles.erase(tex->bindlessHandle);
			tex->bindlessHandle = 0;
			tex->resident = false;
		}

		if (tex->glID) {
			glDeleteTextures(1, &tex->glID);
			tex->glID = 0;
		}

		if (!tex->name.empty()) m_nameMap.erase(tex->name);
		std::erase_if(m_allTextures,
		              [&](const std::shared_ptr<Texture>& t){ return t.get() == tex.get(); });
    }

    void TextureManager::releaseAll() {
		for (const auto h : residentHandles)
			if (h) glMakeTextureHandleNonResidentARB(h);
		residentHandles.clear();

		for (const auto& tex : m_allTextures)
			if (tex && tex->glID) glDeleteTextures(1, &tex->glID);

		m_allTextures.clear();
		m_nameMap.clear();
		m_pathMap.clear();
		m_defaultTextures.clear();
    }

    void TextureManager::loadFromFolder(const std::string &folderName, const std::string &folderPath) {
		namespace fs = std::filesystem;

		if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
			Warn("Texture folder does not exist: " + folderPath);
			return;
		}

		std::unordered_map<std::string, MaterialTextureType> filenameMap = {
			{"basecolor", MaterialTextureType::BaseColor},
			{"albedo",    MaterialTextureType::BaseColor},
			{"normal",    MaterialTextureType::Normal},
			{"roughness", MaterialTextureType::Roughness},
			{"metallic",  MaterialTextureType::Metallic},
			{"ao",        MaterialTextureType::AmbientOcclusion},
			{"emissive",  MaterialTextureType::Emissive}
		};

		for (auto &entry : fs::directory_iterator(folderPath)) {
			if (!entry.is_regular_file()) continue;

			std::string filename = entry.path().filename().string();
			std::string lower = filename;
			std::ranges::transform(lower, lower.begin(), ::tolower);

			for (auto &[key, type] : filenameMap) {
				if (lower.find(key) != std::string::npos) {
					std::string texName = folderName + "_" + key; // e.g., Asphalt031_basecolor
					(void)load(texName, entry.path().string());
				}
			}
		}
    }

    std::shared_ptr<Texture> TextureManager::CreateDefaultTexture(const std::string &debugName,
                                                                  unsigned char r, unsigned char g,
                                                                  unsigned char b, unsigned char a) {
		GLuint texID = 0;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		const unsigned char pixel[4] = { r, g, b, a };
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		auto tex = std::make_shared<Texture>();
		tex->name = debugName;
		tex->glID = texID;

		if (GLAD_GL_ARB_bindless_texture && GLAD_GL_ARB_gpu_shader_int64) {
			if (const uint64_t handle = glGetTextureHandleARB(texID)) {
				glMakeTextureHandleResidentARB(handle);
				tex->bindlessHandle = handle;
				tex->resident = true;
				residentHandles.insert(handle);
			}
		}

		return tex;
    }
}
