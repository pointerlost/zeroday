//
// Created by pointerlost on 8/9/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "Graphics/OpenGL/Shader.h"

namespace Zeroday {

    class AssetManager {
    public:
        static Shader* GetShader(const std::string& name);

        [[nodiscard]] bool LoadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
        [[nodiscard]] static bool CompileComputeShader(const std::string& name, const std::string& filePath);
        [[nodiscard]] bool LoadAllShaders();

    private:
        inline static std::unordered_map<std::string, Shader> g_shaders;
    };
}
