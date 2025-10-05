#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>
#include "glad/glad.h"


namespace Zeroday {

    struct Vertex {
        glm::vec3 m_Position;
        glm::vec3 m_Normal;
        glm::vec2 m_UV;
    };

    struct SubMeshInfo {
        uint32_t m_VertexOffset;
        uint32_t m_IndexOffset;
        uint32_t m_VertexCount;
        uint32_t m_IndexCount;
    };

    class MeshData {
    public:
        // Add proper constructor/destructor
        MeshData() = default;
        ~MeshData() { Cleanup(); }

        // Disable copying (or implement proper resource management)
        MeshData(const MeshData&) = delete;
        MeshData& operator=(const MeshData&) = delete;

        [[nodiscard]] SubMeshInfo& AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i, const std::string& name);
        void UploadToGPU();
        void CreateUniversalVAO();
        void Cleanup();

        // get the mesh information of any primitive type you want in lowercase, ex: ("cube", "triangle")
        [[nodiscard]] SubMeshInfo& GetMeshInfo(const std::string& name) {
            return objectInfo.at(name);
        }

        [[nodiscard]] GLuint GetUniversalVAO() const { return m_UniversalVAO; }
        [[nodiscard]] uint32_t GetVBO() const { return m_VBO; }
        [[nodiscard]] uint32_t GetEBO() const { return m_EBO; }

        [[nodiscard]] const std::vector<Vertex>& GetVertices()  const { return all_Vertices; }
        [[nodiscard]] const std::vector<uint32_t>& GetIndices() const { return all_Indices;  }

        void SetupMeshes();

    private:
        GLuint m_UniversalVAO = 0;
        uint32_t m_VBO = 0;
        uint32_t m_EBO = 0;

        std::vector<Vertex> all_Vertices;
        std::vector<uint32_t> all_Indices;
        std::unordered_map<std::string, SubMeshInfo> objectInfo;
    };

    class MeshData3D : public MeshData {
    public:
        ~MeshData3D() { Cleanup(); }
        void AddMesh3DToMeshData(const std::string& name, std::vector<Vertex>& v, std::vector<uint32_t>& i);

        [[nodiscard]] uint32_t GetIndexCount(const std::string& name) {
            return GetMeshInfo(name).m_IndexCount;
        }

        [[nodiscard]] uint32_t GetIndexOffSet(const std::string& name) {
            return GetMeshInfo(name).m_IndexOffset;
        }
    };
}