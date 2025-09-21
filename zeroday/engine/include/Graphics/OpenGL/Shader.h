//
// Created by pointerlost on 8/9/25.
//
#pragma once
#include <iostream>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Zeroday {
    struct Shader {
        std::string name;
        GLuint program;

        void bind() const;
        void unbind() const { glUseProgram(0); }

        mutable std::unordered_map<std::string, GLuint> uniformCache;

        void cacheUniformLocations() const;
        GLint getUniformLocation(const std::string& uniform) const;

        bool hasUniform(const std::string& uniformName) const;
        void setUniform(const std::string& name, const glm::mat4& type) const;
        void setUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
        void setUniforms(const glm::mat4& mvp) const;

        void setBool(const std::string& name, bool value) const noexcept;
        void setUint(const std::string& name, uint value) const noexcept;
        void setInt(const std::string& name, int value) const noexcept;
        void setFloat(const std::string& name, float value) const noexcept;
        void setVec3(const std::string& name, const glm::vec3& value) const noexcept;
        void setMat4(const std::string& name, const glm::mat4& mat) const noexcept;
        void setMat3(const std::string& name, const glm::mat3& mat) const noexcept;
    };

};