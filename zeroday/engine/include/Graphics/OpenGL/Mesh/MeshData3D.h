#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <functional>
#include <unordered_map>


namespace Zeroday {

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 UV;
	};

	struct SubMeshInfo
	{
		uint32_t vertexOffset;
		uint32_t indexOffset;
		uint32_t vertexCount;
		uint32_t indexCount;

		// VAO ownership
		uint32_t VAO;
	};

	class MeshData
	{
	public:
		[[nodiscard]] SubMeshInfo& AddMesh(std::vector<Vertex> v, std::vector<uint32_t> i, const std::string& name);

		void createVAO(const std::string& subMeshName);
		void uploadToGPU();

		[[nodiscard]] SubMeshInfo& getMeshInfo(const std::string& name) { return objectInfo.at(name); };

		[[nodiscard]] uint32_t getVBO() const { return VBO; };
		[[nodiscard]] uint32_t getEBO() const { return EBO; };

		void setVBO(uint32_t vbo) { VBO = vbo; };
		void setEBO(uint32_t ebo) { EBO = ebo; };

		[[nodiscard]] const std::vector<SubMeshInfo>& getSubMeshInfos() const { return subMeshInfos; };
		[[nodiscard]] const std::vector<Vertex>& getVertices()          const { return all_Vertices; };
		[[nodiscard]] const std::vector<uint32_t>& getIndices()         const { return all_Indices;  };

	protected:
		std::vector<Vertex> all_Vertices;
		std::vector<uint32_t> all_Indices;
		std::vector<SubMeshInfo> subMeshInfos;
		std::unordered_map<std::string, SubMeshInfo> objectInfo;

		uint32_t VBO = 0;
		uint32_t EBO = 0;

		// these counts storting for memory allocation in GPU when the engine is started
		size_t maxVertices = 0;
		size_t maxIndices  = 0;

		// these counts storing for new memory allocations in GPU memory,
		// when the maxVertices and maxIndices counts are insufficient
		size_t currVertexCount = 0;
		size_t currIndexCount  = 0;
	};

	struct MeshData3D : public MeshData
	{
	public:
		~MeshData3D();

		void AddMesh3DToMeshData(const std::string& name, std::vector<Vertex>& v, std::vector<uint32_t>& i);

		[[nodiscard]] uint32_t getIndexCount(const std::string& name)  const { return objectInfo.at(name).indexCount; };
		[[nodiscard]] uint32_t getIndexOffSet(const std::string& name) const { return objectInfo.at(name).indexOffset; };
	};
	
}
