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
		m_DefaultTextures[opengl::MaterialTextureType::BaseColor]        = CreateDefaultTexture("default_basecolor", 255, 255, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Normal]           = CreateDefaultTexture("default_normal",    128, 128, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Roughness]        = CreateDefaultTexture("default_roughness", 255, 255, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Metallic]         = CreateDefaultTexture("default_metallic",    0,   0,   0 );
		m_DefaultTextures[opengl::MaterialTextureType::AmbientOcclusion] = CreateDefaultTexture("default_ao",        255, 255, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Emissive]         = CreateDefaultTexture("default_emissive",    0,   0,   0 );

		// add to maps
		for (auto &tex: m_DefaultTextures | std::views::values) {
			if (tex) {
				m_AllTextures.push_back(tex);
				m_NameMap[tex->m_Name] = tex;
			}
		}
	}

	TextureManager::~TextureManager() {
		// Note: ensure this runs on GL thread or call releaseAll manually on shutdown
		ReleaseAll();
	}

	Ref<Texture> TextureManager::Load(const std::string& name, const std::string &path) {
		if (const auto it  = m_PathMap.find(path); it != m_PathMap.end()) return it->second;

		if (const auto itn = m_NameMap.find(name); itn != m_NameMap.end()) {
			m_PathMap[path] = itn->second;
			return itn->second;
		}

		int width = 0, height = 0, channels = 0;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data) {
			Warn("Failed to load texture: " + path);
			return GetDefaultTexture(opengl::MaterialTextureType::BaseColor);
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

		auto tex = CreateRef<Texture>();
		tex->m_Name = name;
		tex->m_GLId = texID;

		if (GLAD_GL_ARB_bindless_texture && GLAD_GL_ARB_gpu_shader_int64) {
			if (const uint64_t handle = glGetTextureHandleARB(texID)) {
				glMakeTextureHandleResidentARB(handle);
				tex->m_BindlessHandle = handle;
				tex->m_Resident = true;
				m_ResidentHandles.insert(handle);
			}
		}

		m_NameMap[name] = tex;
		m_PathMap[path] = tex;
		m_AllTextures.push_back(tex);
		return tex;
	}

	Ref<Texture> TextureManager::GetTextureWithName(const std::string &name) {
		if (!m_NameMap.contains(name)) {
			Warn("Texture \"" + name + "\" not found.");
			return nullptr;
		}
		return m_NameMap[name];
	}

	Ref<Texture> TextureManager::GetTextureWithPath(const std::string &path) {
		if (!m_PathMap.contains(path)) {
			Warn("Texture \"" + path + "\" not found.");
			return nullptr;
		}
		return m_PathMap[path];
	}

	Ref<Texture> TextureManager::GetDefaultTexture(opengl::MaterialTextureType type) {
		if (const auto it = m_DefaultTextures.find(type); it != m_DefaultTextures.end())
			return it->second;
		return m_DefaultTextures[opengl::MaterialTextureType::BaseColor];
	}

	uint64_t TextureManager::EnsureBindlessHandle(Ref<Texture> tex) {
		if (!tex || tex->m_BindlessHandle) return tex ? tex->m_BindlessHandle : 0;
		if (!GLAD_GL_ARB_bindless_texture || !GLAD_GL_ARB_gpu_shader_int64) return 0;
		if (!glIsTexture(tex->m_GLId)) return 0;

		const uint64_t handle = glGetTextureHandleARB(tex->m_GLId);
		if (!handle) return 0;

		glMakeTextureHandleResidentARB(handle);
		tex->m_BindlessHandle = handle;
		tex->m_Resident = true;
		m_ResidentHandles.insert(handle);
		return handle;
	}

	uint64_t TextureManager::GetBindlessHandle(Ref<Texture> tex) const {
		return tex ? tex->m_BindlessHandle : 0;
	}

    void TextureManager::ReleaseTexture(const Ref<Texture> &tex) {
		if (!tex) return;

		if (tex->m_Resident && tex->m_BindlessHandle) {
			glMakeTextureHandleNonResidentARB(tex->m_BindlessHandle);
			m_ResidentHandles.erase(tex->m_BindlessHandle);
			tex->m_BindlessHandle = 0;
			tex->m_Resident = false;
		}

		if (tex->m_GLId) {
			glDeleteTextures(1, &tex->m_GLId);
			tex->m_GLId = 0;
		}

		if (!tex->m_Name.empty()) m_NameMap.erase(tex->m_Name);
		std::erase_if(m_AllTextures,
		              [&](const Ref<Texture>& t){ return t.get() == tex.get(); });
    }

    void TextureManager::ReleaseAll() {
		for (const auto h : m_ResidentHandles)
			if (h) glMakeTextureHandleNonResidentARB(h);
		m_ResidentHandles.clear();

		for (const auto& tex : m_AllTextures)
			if (tex && tex->m_GLId) glDeleteTextures(1, &tex->m_GLId);

		m_AllTextures.clear();
		m_NameMap.clear();
		m_PathMap.clear();
		m_DefaultTextures.clear();
    }

    void TextureManager::LoadFromFolder(const std::string &folderName, const std::string &folderPath) {
		namespace fs = std::filesystem;

		if (!fs::exists(folderPath) || !fs::is_directory(folderPath)) {
			Warn("Texture folder does not exist: " + folderPath);
			return;
		}

		std::unordered_map<std::string, opengl::MaterialTextureType> filenameMap = {
			{"basecolor", opengl::MaterialTextureType::BaseColor},
			{"albedo",    opengl::MaterialTextureType::BaseColor},
			{"normal",    opengl::MaterialTextureType::Normal},
			{"roughness", opengl::MaterialTextureType::Roughness},
			{"metallic",  opengl::MaterialTextureType::Metallic},
			{"ao",        opengl::MaterialTextureType::AmbientOcclusion},
			{"emissive",  opengl::MaterialTextureType::Emissive}
		};

		for (auto &entry : fs::directory_iterator(folderPath)) {
			if (!entry.is_regular_file()) continue;

			std::string filename = entry.path().filename().string();
			std::string lower = filename;
			std::ranges::transform(lower, lower.begin(), ::tolower);

			for (auto &[key, type] : filenameMap) {
				if (lower.find(key) != std::string::npos) {
					std::string texName = folderName + "_" + key; // e.g., Asphalt031_basecolor
					(void)Load(texName, entry.path().string());
				}
			}
		}
    }

    Ref<Texture> TextureManager::CreateDefaultTexture(const std::string &debugName,
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

		auto tex = CreateRef<Texture>();
		tex->m_Name = debugName;
		tex->m_GLId = texID;

		if (GLAD_GL_ARB_bindless_texture && GLAD_GL_ARB_gpu_shader_int64) {
			if (const uint64_t handle = glGetTextureHandleARB(texID)) {
				glMakeTextureHandleResidentARB(handle);
				tex->m_BindlessHandle = handle;
				tex->m_Resident = true;
				m_ResidentHandles.insert(handle);
			}
		}

		return tex;
    }
}
