//
// Created by pointerlost on 8/15/25.
//
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"

namespace Zeroday::opengl {

    struct MeshEntry {
        std::string m_SubMeshName;
        Ref<MaterialInstance> m_Material;
    };

    struct Model {
        std::vector<MeshEntry> m_Meshes;
        std::string m_Name;
        std::string m_FilePath;

        void AddMesh(const std::string& name, Ref<MaterialInstance> material) {
            m_Meshes.push_back({name, std::move(material)});
        }

        [[nodiscard]] size_t GetMeshCount() const { return m_Meshes.size(); }
        [[nodiscard]] bool IsValid() const { return !m_Meshes.empty(); }
    };
}
