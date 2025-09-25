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
        std::string m_Name;
        GLuint m_Program;

        void Bind() const;
        void UnBind() const { glUseProgram(0); }

        mutable std::unordered_map<std::string, GLuint> uniformCache;

        void CacheUniformLocations() const;
        GLint GetUniformLocation(const std::string& uniform) const;

        bool HasUniform(const std::string& uniformName) const;
        void SetUniform(const std::string& name, const glm::mat4& type) const;
        void setUniforms(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const;
        void setUniforms(const glm::mat4& mvp) const;

        void SetBool(const std::string& name, bool value) const noexcept;
        void SetUint(const std::string& name, uint value) const noexcept;
        void SetInt(const std::string& name, int value) const noexcept;
        void SetFloat(const std::string& name, float value) const noexcept;
        void SetVec3(const std::string& name, const glm::vec3& value) const noexcept;
        void SetMat4(const std::string& name, const glm::mat4& mat) const noexcept;
        void SetMat3(const std::string& name, const glm::mat3& mat) const noexcept;
    };

};