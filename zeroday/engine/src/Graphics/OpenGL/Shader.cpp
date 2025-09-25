//
// Created by pointerlost on 8/9/25.
//
#include "Graphics/OpenGL/Shader.h"
#include <core/Logger.h>

namespace Zeroday {

    void Shader::Bind() const {
        glUseProgram(m_Program);
    }

    void Shader::CacheUniformLocations() const {
        GLint count;
        glGetProgramiv(m_Program, GL_ACTIVE_UNIFORMS, &count);

        char name[256];
        for (GLint i = 0; i < count; i++) {
            GLsizei length;
            GLint size;
            GLenum type;
            glGetActiveUniform(m_Program, i, sizeof(name), &length, &size, &type, name);

            const GLint location = glGetUniformLocation(m_Program, name);
            uniformCache[name] = location;
        }
    }

    GLint Shader::GetUniformLocation(const std::string &uniform) const {
        if (uniformCache.contains(uniform))
            return uniformCache[uniform];
        return -1;
    }

    bool Shader::HasUniform(const std::string &uniformName) const {
        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

        if (currentProgram != m_Program) {
            Error("Shader not bound when checking uniform: " + uniformName +
                " | expected: " + std::to_string(m_Program) +
                " | current: " + std::to_string(currentProgram));
            return false;
        }
        return true;
    }

    void Shader::SetUniform(const std::string &name, const glm::mat4 &type) const {
        SetMat4(name, type);
    }

    void Shader::setUniforms(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) const {
        const glm::mat4& mvp = projection * view * model;
        SetMat4("MVP", mvp);
    }

    void Shader::setUniforms(const glm::mat4 &mvp) const {
		SetMat4("MVP", mvp);
    }

    void Shader::SetBool(const std::string &name, bool value) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniform1i(loc, static_cast<int>(value));
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetUint(const std::string &name, uint value) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniform1ui(loc, value);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetInt(const std::string &name, int value) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniform1i(loc, value);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetFloat(const std::string &name, float value) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniform1f(loc, value);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniform3fv(loc, 1, &value[0]);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }

    void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const noexcept {
        if (const GLint loc = GetUniformLocation(name); loc != -1)
            glUniformMatrix3fv(loc, 1, GL_FALSE, &mat[0][0]);
        else
            Warn("[Warning] uniform '" + name + "' not found or optimized out!");
    }
}
