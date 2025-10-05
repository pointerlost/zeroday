#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include <cstring>
#include <ranges>
#include <glad/glad.h>
#include "Core/Logger.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/Mesh/MeshFactory.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"

namespace Zeroday {

    void MeshData::UploadToGPU() {
        // Create and upload VBO
        glGenBuffers(1, &m_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     all_Vertices.size() * sizeof(Vertex),
                     all_Vertices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create and upload EBO
        glGenBuffers(1, &m_EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     all_Indices.size() * sizeof(uint32_t),
                     all_Indices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    SubMeshInfo& MeshData::AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i, const std::string& name) {
        SubMeshInfo info{};
        info.m_VertexOffset = all_Vertices.size();
        info.m_IndexOffset = all_Indices.size();
        info.m_VertexCount = v.size();
        info.m_IndexCount = i.size();

        // Add vertices
        all_Vertices.insert(all_Vertices.end(), v.begin(), v.end());

        // Append indices but adjust by vertexOffset
        for (const uint32_t idx : i) {
            all_Indices.push_back(idx + info.m_VertexOffset);
        }

        objectInfo[name] = info;
        return objectInfo[name];
    }

    void MeshData::CreateUniversalVAO() {
        if (m_UniversalVAO != 0) {
            glDeleteVertexArrays(1, &m_UniversalVAO);
        }

        glCreateVertexArrays(1, &m_UniversalVAO);

        // direct state access
        // Bind VBO to VAO's vertex buffer binding point
        glVertexArrayVertexBuffer(m_UniversalVAO, 0, m_VBO, 0, sizeof(Vertex));

        // Bind EBO to VAO
        glVertexArrayElementBuffer(m_UniversalVAO, m_EBO);

        // Set up vertex attributes
        // Position (location = 0)
        glEnableVertexArrayAttrib(m_UniversalVAO, 0);
        glVertexArrayAttribFormat(m_UniversalVAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Position));
        glVertexArrayAttribBinding(m_UniversalVAO, 0, 0);

        // Normal (location = 1)
        glEnableVertexArrayAttrib(m_UniversalVAO, 1);
        glVertexArrayAttribFormat(m_UniversalVAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_Normal));
        glVertexArrayAttribBinding(m_UniversalVAO, 1, 0);

        // UV (location = 2)
        glEnableVertexArrayAttrib(m_UniversalVAO, 2);
        glVertexArrayAttribFormat(m_UniversalVAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, m_UV));
        glVertexArrayAttribBinding(m_UniversalVAO, 2, 0);

        Info("Created Universal VAO: " + std::to_string(m_UniversalVAO) +
             " with " + std::to_string(all_Vertices.size()) + " vertices");
    }

    void MeshData::Cleanup() {
        if (m_UniversalVAO != 0) {
            glDeleteVertexArrays(1, &m_UniversalVAO);
            m_UniversalVAO = 0;
        }
        if (m_VBO != 0) {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
        if (m_EBO != 0) {
            glDeleteBuffers(1, &m_EBO);
            m_EBO = 0;
        }
    }

    void MeshData::SetupMeshes() {
        const auto AddMeshObject = [&](const std::string &name) {
            auto [vertices, indices] = MeshFactory::CreateMeshObject(name);
            Services::GetMeshLibrary()->GetMeshData3D()->AddMesh3DToMeshData(name, vertices, indices);
        };

        AddMeshObject("triangle");
        AddMeshObject("square");
        AddMeshObject("cube");
        AddMeshObject("circle");
        AddMeshObject("sphere");

        // Creation VBO, EBO
        UploadToGPU();
        CreateUniversalVAO();

        Info("[MeshFactory::setupMeshes] successful!");
    }

    void MeshData3D::AddMesh3DToMeshData(const std::string& name,
                                         std::vector<Vertex>& v,
                                         std::vector<uint32_t>& i) {
        (void)AddMesh(std::move(v), std::move(i), name);
    }

}
