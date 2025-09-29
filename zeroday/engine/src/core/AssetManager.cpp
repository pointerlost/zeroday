//
// Created by pointerlost on 8/9/25.
//
#include "core/AssetManager.h"
#include <core/Logger.h>
#include <core/Config.h>
#include <core/File.h>
#include <nlohmann/json.hpp>

namespace Zeroday {

    Shader* AssetManager::GetShader(const std::string &name) {
        if (!g_Shaders.contains(name)) {
            Warn("[AssetManager::getShader] shader not found!");
            return {};
        }
        return &g_Shaders[name];
    }

    bool AssetManager::LoadShader(const std::string &name, const std::string &vertPath, const std::string &fragPath) {
        auto& file = File::get();

        const auto fullVertPath = std::string(SHADERS_DIR) + vertPath;
        const auto fullFragPath = std::string(SHADERS_DIR) + fragPath;

        // Read original sources
        auto vertSource = file.readFromFile(fullVertPath);
        auto fragSource = file.readFromFile(fullFragPath);

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
        auto& file = File::get();

        const auto fullPath = std::string(SHADERS_DIR) + "opengl/compute/" + filePath;
        auto source = file.readFromFile(fullPath);

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

        auto& file = File::get();

        std::string jsonContent  = file.readFromFile(configPath);
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
                    auto& file = File::get();
                    std::string includeContent = file.readFromFile(includePath);

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
}
