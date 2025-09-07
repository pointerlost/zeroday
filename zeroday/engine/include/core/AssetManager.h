//
// Created by pointerlost on 8/9/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include "../Graphics/OpenGL/Shader.h"

namespace ASSET {
    class AssetManager {
    public:
        static Graphics::Shader* getShader(const std::string& name);

        bool loadShader(const std::string& name, const std::string& vertPath, const std::string& fragPath);
        bool loadAllShaders();

    private:
        inline static std::unordered_map<std::string, Graphics::Shader> shaders;
    };
}

