//
// Created by pointerlost on 8/9/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "Graphics/OpenGL/Shader.h"

namespace Zeroday::Asset {

    class AssetManager {
    public:
        static Zeroday::Shader* getShader(const std::string& name);

        [[nodiscard]] bool loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
        [[nodiscard]] static bool CompileComputeShader(const std::string& name, const std::string& filePath);
        [[nodiscard]] bool loadAllShaders();

    private:
        inline static std::unordered_map<std::string, Zeroday::Shader> g_shaders;
    };
}

