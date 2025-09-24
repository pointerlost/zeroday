//
// Created by pointerlost on 9/23/25.
//
#include "Scene/Components.h"
#include "Graphics/OpenGL/Material/material.h"

namespace Zeroday {

    opengl::MaterialSSBO MaterialComponent::ToGPUFormat() const {
        if (m_Instance) {
            return m_Instance->ToGPUFormat();
        }
        // Return default material if no instance
        opengl::MaterialSSBO defaultMat;
        defaultMat.baseColor = glm::vec4(1.0f);
        defaultMat.metallic = 0.0f;
        defaultMat.roughness = 0.5f;
        defaultMat.emissive = glm::vec3(0.0f);
        return defaultMat;
    }
}
