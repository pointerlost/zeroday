//
// Created by pointerlost on 8/9/25.
//
#include "Core/AssetManager.h"
#include <Core/Logger.h>
#include <Core/Config.h>
#include <Core/File.h>
#include <nlohmann/json.hpp>
#include "Core/Assert.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Core/Base.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "stb/stb_image.h"

static Zeroday::File& file = Zeroday::File::Get();
using json = nlohmann::json;

namespace Zeroday {

    AssetManager::AssetManager() {
        CreateDefaultTextures();
    }

    Shader* AssetManager::GetShader(const std::string &name) {
        if (!g_Shaders.contains(name)) {
            Warn("[AssetManager::getShader] shader not found!");
            return {};
        }
        return &g_Shaders[name];
    }

    bool AssetManager::LoadShader(const std::string &name, const std::string &vertPath, const std::string &fragPath) {
        auto& file = File::Get();

        const auto fullVertPath = std::string(SHADERS_DIR) + vertPath;
        const auto fullFragPath = std::string(SHADERS_DIR) + fragPath;

        // Read original sources
        auto vertSource = file.ReadFromFile(fullVertPath);
        auto fragSource = file.ReadFromFile(fullFragPath);

        // FIX: Use the correct base path - shaders/opengl/
        std::string shaderBaseDir = std::string(SHADERS_DIR) + "opengl/";

        // Preprocess shaders (resolve includes)
        std::string processedVertSource = PreprocessShader(vertSource, shaderBaseDir);
        std::string processedFragSource = PreprocessShader(fragSource, shaderBaseDir);

        // Debug: Print processed shaders (optional)
        #ifdef DEBUG_SHADERS
        std::cout << "=== Processed Vertex Shader: " << name << " ===" << std::endl;
        std::cout << processedVertSource << std::endl;
        std::cout << "=== Processed Fragment Shader: " << name << " ===" << std::endl;
        std::cout << processedFragSource << std::endl;
        #endif

        int success = 0;
        char infoLog[1024];

        const char* vertexSourceCode = processedVertSource.c_str();
        const char* fragSourceCode   = processedFragSource.c_str();

        const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceCode, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Vertex shader compilation error: " + std::string(infoLog));
            return false;
        }

        const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragSourceCode, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Fragment shader compilation error: " + std::string(infoLog));
            glDeleteShader(vertexShader);
            return false;
        }

        const uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return false;
        }

        // Clean up shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Shader instance;
        instance.m_Name = name;
        instance.m_Program = shaderProgram;
        instance.CacheUniformLocations();

        g_Shaders[name] = std::move(instance);
        return true;
    }

    bool AssetManager::CompileComputeShader(const std::string& name, const std::string &filePath) {
        auto& file = File::Get();

        const auto fullPath = std::string(SHADERS_DIR) + "opengl/compute/" + filePath;
        auto source = file.ReadFromFile(fullPath);

        // FIX: Add preprocessing with correct base path
        std::string shaderBaseDir = std::string(SHADERS_DIR) + "opengl/";
        std::string processedSource = PreprocessShader(source, shaderBaseDir);

        const char* sourceCode = processedSource.c_str();

        int success = 0;
        char infoLog[1024];

        const uint32_t computeShader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(computeShader, 1, &sourceCode, nullptr);
        glCompileShader(computeShader);
        glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(computeShader, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            return false;
        }

        const uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, computeShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            return false;
        }

        // clean to avoid GPU resource leaks
        glDeleteShader(computeShader);

        Shader instance;
        instance.m_Name = name;
        instance.m_Program = shaderProgram;
        instance.CacheUniformLocations();

        g_Shaders[name] = std::move(instance);
        return true;
    }

    bool AssetManager::LoadAllShaders() {
        bool overallSuccess = true;

        const std::string configPath = SHADERS_CONFIG_PATH;
        Info("Loading shaders from config: " + configPath);

        auto& file = File::Get();

        std::string jsonContent  = file.ReadFromFile(configPath);
        auto config = nlohmann::json::parse(jsonContent);

        if (!config.contains("shaders")) {
            throw std::runtime_error("Missing 'shaders' array in config");
        }

        for (auto& shaderConfig : config["shaders"]) {
            std::string name     = shaderConfig.value("name", "unnamed");
            std::string type     = shaderConfig.value("type", "unnamed");
            std::string vertexPath, fragmentPath, computePath;
            if (shaderConfig.contains("stages")) {
                vertexPath   = shaderConfig["stages"].value("vertex",   "unnamed");
                fragmentPath = shaderConfig["stages"].value("fragment", "unnamed");
                computePath  = shaderConfig["stages"].value("compute",  "unnamed");
            } else {
                Error("[ShaderManager::loadAllShaders] missing 'stages' array in shaders config!");
                return false;
            }

            if (type == "COMPUTE") {
                if (!CompileComputeShader(name, computePath)) {
                    Error("Failed to load compute shader: " + name);
                    overallSuccess = false;
                    continue; // Skip to next shader
                }
            } else {
                if (!LoadShader(name, vertexPath, fragmentPath)) {
                    Error("Failed to load shader: " + name);
                    overallSuccess = false;
                    continue;
                }
            }
            Info("Shader: " + name + " loaded successfully!");
        }

        return overallSuccess;
    }

    std::string AssetManager::PreprocessShader(const std::string& source, const std::string& basePath) {
        std::stringstream result;
        std::stringstream ss(source);
        std::string line;

        while (std::getline(ss, line)) {
            // Check for include directive
            if (line.find("#include") != std::string::npos) {
                // Extract filename from include
                size_t start = line.find('"');
                size_t end = line.rfind('"');
                if (start != std::string::npos && end != std::string::npos && start != end) {
                    std::string filename = line.substr(start + 1, end - start - 1);

                    // FIX: Use the basePath parameter correctly
                    std::string includePath = basePath + filename;

                    // Read included file
                    auto& file = File::Get();
                    std::string includeContent = file.ReadFromFile(includePath);

                    if (!includeContent.empty()) {
                        result << includeContent << "\n";
                    } else {
                        Error("Could not open include file: " + includePath);
                    }
                }
            } else {
                result << line << "\n";
            }
        }

        return result.str();
    }

    bool AssetManager::LoadMaterialsFromFolder(const std::string& filePath) {
        std::string content = file.ReadFromFile(filePath);
        if (content.empty()) {
            Warn("[MaterialLibrary::CreateMaterials] Content is empty for file: " + filePath + "\n");
            return false;
        }

        json parsed;
        try {
            parsed = json::parse(content);
        } catch (const json::parse_error& e) {
            Warn("[MaterialLibrary::CreateMaterials] JSON parse error: " + std::string(e.what()) + "\n");
            return false;
        }

        if (!parsed.contains("materials") || !parsed["materials"].is_array()) {
            Warn("[MaterialLibrary::CreateMaterials] Invalid material file structure! Missing 'materials' array.\n");
            return false;
        }

        int createdCount = 0;

        for (auto& mat : parsed["materials"]) {
            if (!mat.contains("name") || !mat["name"].is_string()) {
                Warn("[MaterialLibrary::CreateMaterials] Material missing name, skipping...");
                continue;
            }

            std::string name = mat["name"].get<std::string>();

            // Fallback values
            glm::vec4 baseColor(1.0f);
            glm::vec3 emissive(0.0f);
            float metallic = 0.0f;
            float roughness = 1.0f;

            // Parse material properties
            if (mat.contains("baseColor") && mat["baseColor"].is_array() && mat["baseColor"].size() >= 3) {
                try {
                    baseColor.r = mat["baseColor"][0].get<float>();
                    baseColor.g = mat["baseColor"][1].get<float>();
                    baseColor.b = mat["baseColor"][2].get<float>();
                    baseColor.a = (mat["baseColor"].size() >= 4) ? mat["baseColor"][3].get<float>() : 1.0f;
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid baseColor for material '" + name + "', using default.");
                }
            }

            if (mat.contains("emissive") && mat["emissive"].is_array() && mat["emissive"].size() == 3) {
                try {
                    emissive.r = mat["emissive"][0].get<float>();
                    emissive.g = mat["emissive"][1].get<float>();
                    emissive.b = mat["emissive"][2].get<float>();
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid emissive for material '" + name + "', using default.");
                }
            }

            if (mat.contains("metallic")) {
                try {
                    metallic = std::clamp(mat["metallic"].get<float>(), 0.0f, 1.0f);
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid metallic for material '" + name + "', using default.");
                }
            }

            if (mat.contains("roughness")) {
                try {
                    roughness = std::clamp(mat["roughness"].get<float>(), 0.0f, 1.0f);
                } catch (const json::exception& e) {
                    Warn("[MaterialLibrary::CreateMaterials] Invalid roughness for material '" + name + "', using default.");
                }
            }

            // Create material
            auto material = CreateRef<opengl::Material>();
            material->baseColor = baseColor;
            material->emissive  = emissive;
            material->metallic  = metallic;
            material->roughness = roughness;

            // Load textures if they exist
            if (mat.contains("textures") && mat["textures"].is_object()) {
                auto& textures = mat["textures"];

                // Helper to load texture
                auto loadTexture = [&](const std::string& texturePath, opengl::MaterialTextureType type) {
                    if (!texturePath.empty()) {
                        try {
                            auto fullTexturePath = TEXTURES_DIR + texturePath;
                            // Try to get existing texture first
                            auto texture = GetTextureWithPath(fullTexturePath);
                            if (!texture) {
                                // Load new texture
                                const std::string texName = name + "_" + GetTextureTypeName(type);
                                texture = Load(texName, fullTexturePath);
                            }
                            if (texture) {
                                material->textures[type] = texture;
                            }
                        } catch (const std::exception& e) {
                            Warn("[MaterialLibrary::CreateMaterials] Failed to load texture '" +
                                std::string(TEXTURES_DIR) + texturePath + "' for material '" + name + "': " + e.what());
                        }
                    }
                };

                // Load each texture type
                if (textures.contains("baseColor") && textures["baseColor"].is_string()) {
                    loadTexture(textures["baseColor"].get<std::string>(), opengl::MaterialTextureType::BaseColor);
                }
                if (textures.contains("normal") && textures["normal"].is_string()) {
                    loadTexture(textures["normal"].get<std::string>(), opengl::MaterialTextureType::Normal);
                }
                if (textures.contains("roughness") && textures["roughness"].is_string()) {
                    loadTexture(textures["roughness"].get<std::string>(), opengl::MaterialTextureType::Roughness);
                }
                if (textures.contains("displacement") && textures["displacement"].is_string()) {
                    loadTexture(textures["displacement"].get<std::string>(), opengl::MaterialTextureType::Displacement);
                }
                if (textures.contains("ambientOcclusion") && textures["ambientOcclusion"].is_string()) {
                    loadTexture(textures["ambientOcclusion"].get<std::string>(), opengl::MaterialTextureType::AmbientOcclusion);
                }
            }

            // Set default textures for any missing ones
            auto setDefaultIfMissing = [&](opengl::MaterialTextureType type) {
                if (!material->textures.contains(type)) {
                    material->textures[type] = GetDefaultTexture(type);
                }
            };

            setDefaultIfMissing(opengl::MaterialTextureType::BaseColor);
            setDefaultIfMissing(opengl::MaterialTextureType::Normal);
            setDefaultIfMissing(opengl::MaterialTextureType::Roughness);
            setDefaultIfMissing(opengl::MaterialTextureType::Displacement);
            setDefaultIfMissing(opengl::MaterialTextureType::AmbientOcclusion);

            m_Materials[name] = material;
            createdCount++;
            Info("Created material: " + name + " with " + std::to_string(material->textures.size()) + " textures");
        }

        Info("[MaterialLibrary::CreateMaterials] Successfully created " + std::to_string(createdCount) + " materials from " + filePath);
        return createdCount > 0;
    }

    Ref<opengl::Material> AssetManager::GetMaterialByName(const std::string &name) {
        const auto it = m_Materials.find(name);
        if (it == m_Materials.end()) {
            Warn("Material '" + name + "' not found! Returning default material.\n");
            return GetDefaultMaterial();
        }
        return it->second;
    }

    Ref<opengl::Material> AssetManager::GetDefaultMaterial() {
        static Ref<opengl::Material> fallback = [this]{
            auto mat = CreateRef<opengl::Material>();
            mat->baseColor = glm::vec4(1.0f);
            mat->emissive  = glm::vec3(0.0f);
            mat->metallic  = 0.0f;
            mat->roughness = 1.0f;

            mat->textures[opengl::MaterialTextureType::BaseColor]        = GetDefaultTexture(opengl::MaterialTextureType::BaseColor);
            mat->textures[opengl::MaterialTextureType::Normal]           = GetDefaultTexture(opengl::MaterialTextureType::Normal);
            mat->textures[opengl::MaterialTextureType::Roughness]        = GetDefaultTexture(opengl::MaterialTextureType::Roughness);
            mat->textures[opengl::MaterialTextureType::Displacement]     = GetDefaultTexture(opengl::MaterialTextureType::Displacement);
            mat->textures[opengl::MaterialTextureType::AmbientOcclusion] = GetDefaultTexture(opengl::MaterialTextureType::AmbientOcclusion);

            Info("Default Material added!");
            return mat;
        }();
        return fallback;
    }

    void AssetManager::CreateDefaultTextures() {
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

    Ref<Texture> AssetManager::CreateDefaultTexture(const std::string &debugName, unsigned char r, unsigned char g,
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

    Ref<Texture> AssetManager::GetDefaultTexture(opengl::MaterialTextureType type) {
        if (const auto it = m_DefaultTextures.find(type); it != m_DefaultTextures.end())
            return it->second;
        return m_DefaultTextures[opengl::MaterialTextureType::BaseColor];
    }

    std::string AssetManager::GetTextureTypeName(opengl::MaterialTextureType type) {
        switch (type) {
            case opengl::MaterialTextureType::BaseColor:        return "baseColor";
            case opengl::MaterialTextureType::Normal:           return "normal";
            case opengl::MaterialTextureType::Roughness:        return "roughness";
            case opengl::MaterialTextureType::Displacement:     return "displacement";
            case opengl::MaterialTextureType::AmbientOcclusion: return "ao";
            default: return "unknown";
        }
    }

    Ref<Texture> AssetManager::GetTextureWithName(const std::string &name) {
        if (!m_NameMap.contains(name)) {
            Warn("Texture \"" + name + "\" not found.");
            return nullptr;
        }
        return m_NameMap[name];
    }

    Ref<Texture> AssetManager::GetTextureWithPath(const std::string &path) {
        if (!m_PathMap.contains(path)) {
            Warn("Texture \"" + path + "\" not found.");
            return nullptr;
        }
        return m_PathMap[path];
    }

    Ref<Texture> AssetManager::Load(const std::string &name, const std::string &path) {
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

    uint64_t AssetManager::GetBindlessHandle(Ref<Texture> tex) const {
		return tex ? tex->m_BindlessHandle : 0;
    }

    void AssetManager::ReleaseAllTextureHandles() {
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

    Ref<opengl::MaterialInstance> AssetManager::CreateMaterialInstance(const std::string &name) {
        const auto base = GetMaterialByName(name);
        auto instance  = CreateRef<opengl::MaterialInstance>();
        instance->m_Base = base;
        return instance;
    }

}
