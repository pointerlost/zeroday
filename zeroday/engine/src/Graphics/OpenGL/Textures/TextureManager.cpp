//
// Created by pointerlost on 8/15/25.
//
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include <algorithm>
#include <filesystem>
#include "core/Logger.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Graphics/OpenGL/Textures/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nlohmann/detail/string_concat.hpp>
#include "core/Assert.h"
#include "core/File.h"
#include "Graphics/OpenGL/Material/material.h"

static Zeroday::File& fileManager = Zeroday::File::get();

namespace Zeroday
{
	TextureManager::TextureManager() {
		// Modern PBR default textures (1x1 pixel)
		m_DefaultTextures[opengl::MaterialTextureType::BaseColor]        = CreateDefaultTexture("default_basecolor", 255, 255, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Normal]           = CreateDefaultTexture("default_normal",    128, 128, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Roughness]        = CreateDefaultTexture("default_roughness", 255, 255, 255 );
		m_DefaultTextures[opengl::MaterialTextureType::Displacement]     = CreateDefaultTexture("default_metallic",    0,   0,   0 );
		m_DefaultTextures[opengl::MaterialTextureType::AmbientOcclusion] = CreateDefaultTexture("default_ao",        255, 255, 255 );

		for (const auto& tex: m_DefaultTextures | std::views::values) {
			tex->MakeResident();
		}
	}

	TextureManager::~TextureManager() {
		// ensure this runs on GL thread or call releaseAll manually on shutdown
		ReleaseAll();
	}

	Ref<Texture> TextureManager::Load(const std::string& name, const std::string &path) {
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data) {
			ZD_ASSERT(false, "Failed to load texture: " + path);
			throw std::runtime_error("Failed to load texture: " + path);
		}

		// Determine format
		GLenum format, internalFormat;
		if (channels == 4) {
			format = GL_RGBA;
			internalFormat = GL_RGBA8;
		}
		else if (channels == 3) {
			format = GL_RGB;
			internalFormat = GL_RGB8;
		}
		else if (channels == 1) {
			format = GL_RED;
			internalFormat = GL_R8;
		}
		else {
			stbi_image_free(data);
			throw std::runtime_error("[TextureManager::Load] channel size mismatch!");
		}

		// create Texture using DSA
		auto texture = CreateRef<Texture>();
		texture->Create(width, height, internalFormat);
		texture->UploadData(format, GL_UNSIGNED_BYTE, data);

		// store textures
		texture->m_Name = name;
		m_NameMap[name] = texture;
		m_PathMap[path] = texture;

		// make it bindless resident
		texture->MakeResident();

		stbi_image_free(data);
		return texture;
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

	std::string TextureManager::GetTextureTypeName(opengl::MaterialTextureType type) {
		switch (type) {
			case opengl::MaterialTextureType::BaseColor:        return "baseColor";
			case opengl::MaterialTextureType::Normal:           return "normal";
			case opengl::MaterialTextureType::Roughness:        return "roughness";
			case opengl::MaterialTextureType::Displacement:     return "displacement";
			case opengl::MaterialTextureType::AmbientOcclusion: return "ao";
			default: return "unknown";
		}
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

		if (tex->m_Id) {
			glDeleteTextures(1, &tex->m_Id);
			tex->m_Id = 0;
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
			if (tex && tex->m_Id) glDeleteTextures(1, &tex->m_Id);

		m_AllTextures.clear();
		m_NameMap.clear();
		m_PathMap.clear();
		m_DefaultTextures.clear();
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
		tex->m_Id = texID;

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
