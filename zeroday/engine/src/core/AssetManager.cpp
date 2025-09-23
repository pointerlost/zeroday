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
        if (!g_shaders.contains(name)) {
            Warn("[AssetManager::getShader] shader not found!");
            return {};
        }
        return &g_shaders[name];
    }

    bool AssetManager::LoadShader(const std::string &name, const std::string &vertPath, const std::string &fragPath) {
        auto& file = File::get();

        const auto fullVertPath = std::string(SHADERS_DIR) + "/" + vertPath;
        const auto fullFragPath = std::string(SHADERS_DIR) + "/" + fragPath;

        const auto vertSource = file.readFromFile(fullVertPath);
        const auto fragSource = file.readFromFile(fullFragPath);

        int success = 0;
        char infoLog[1024];

        const char* vertexSourceCode = vertSource.c_str();
        const char* fragSourceCode   = fragSource.c_str();

        const uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceCode, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            return false;
        }

        const uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragSourceCode, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            return false;
        }

        const uint32_t shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, 1024, nullptr, infoLog);
            Error("[GLShaderProgram] Program linking error: " + std::string(infoLog));
            return false;
        }

        // clean to avoid GPU resource leaks
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        Shader instance;
        instance.name = name;
        instance.program = shaderProgram;
        instance.cacheUniformLocations();

        g_shaders[name] = std::move(instance);
        return true;
    }

    bool AssetManager::CompileComputeShader(const std::string& name, const std::string &filePath) {
        auto& file = File::get();

        const auto fullPath = std::string(COMPUTE_SHADERS_DIR) + "/" + filePath;
        const auto source   = file.readFromFile(fullPath);

        int success = 0;
        char infoLog[1024];

        const char* sourceCode   = source.c_str();

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
        instance.name = name;
        instance.program = shaderProgram;
        instance.cacheUniformLocations();

        g_shaders[name] = std::move(instance);
        return true;
    }

    bool AssetManager::LoadAllShaders() {
        const std::string configPath = SHADERS_CONFIG_PATH;
        Info("Loading shaders from config: " + configPath);

        auto& file = File::get();

        try {
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

                if (type == "COMPUTE" && !CompileComputeShader(name, computePath)) {
                    Warn("Loading compute shader from " + name + " failed");
                    return false;
                }
                if (!LoadShader(name, vertexPath, fragmentPath)) {
                    Error("Failed to load shader: " + name);
                    return false;
                }
                Info("Shader: " + name + " loaded successfully!");
            }
            return true;
        }
        catch (...) {
            Error("Failed to load shader config!");
            return false;
        }
    }
}
