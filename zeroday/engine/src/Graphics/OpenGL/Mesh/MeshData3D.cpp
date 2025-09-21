#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include <cstring>
#include <glad/glad.h>

#include "core/Logger.h"

namespace Zeroday {

    void MeshData::uploadToGPU()
    {
        // Create and upload VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     all_Vertices.size() * sizeof(Vertex),
                     all_Vertices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Create and upload EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     all_Indices.size() * sizeof(uint32_t),
                     all_Indices.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void MeshData::createVAO(const std::string& subMeshName)
    {
        auto& subMesh = getMeshInfo(subMeshName);

        if (subMesh.VAO != 0) return;

        glGenVertexArrays(1, &subMesh.VAO);
        glBindVertexArray(subMesh.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, position)));

        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, normal)));

        // UV attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                             reinterpret_cast<void*>(offsetof(Vertex, UV)));

        Info("Created VAO: " + std::to_string(subMesh.VAO) +
                     " for submesh: " + subMeshName);

        // Unbind VAO
        glBindVertexArray(0);
    }

    SubMeshInfo& MeshData::AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i, const std::string& name)
    {
        SubMeshInfo info{};
        info.vertexOffset = all_Vertices.size();
        info.indexOffset = all_Indices.size();
        info.vertexCount = v.size();
        info.indexCount = i.size();

        // Add vertices
        all_Vertices.insert(all_Vertices.end(), v.begin(), v.end());

        // Add indices with offset adjustment
        all_Indices.reserve(all_Indices.size() + i.size());
        for (const auto idx : i)
        {
            all_Indices.push_back(idx + info.vertexOffset);
        }

        subMeshInfos.push_back(info);
        objectInfo[name] = info;
        createVAO(name);

        return subMeshInfos.back();
    }

    MeshData3D::~MeshData3D()
    {
        for (auto& info : subMeshInfos)
        {
            if (info.VAO != 0)
            {
                glDeleteVertexArrays(1, &info.VAO);
                info.VAO = 0;
            }
        }

        // Consider also deleting VBO and EBO here if appropriate
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void MeshData3D::AddMesh3DToMeshData(const std::string& name,
                                                std::vector<Vertex>& v,
                                                std::vector<uint32_t>& i)
    {
        (void)AddMesh(std::move(v), std::move(i), name);
    }

}
