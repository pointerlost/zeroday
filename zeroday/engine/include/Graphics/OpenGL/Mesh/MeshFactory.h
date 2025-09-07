#pragma once
#include <memory>
#include <unordered_map>
#include <functional>

namespace Graphics
{
	struct Vertex;
	struct MeshData3D;

	class MeshFactory
	{
	public:
		MeshFactory();

		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createTriangle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createSquare();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createCube();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createCircle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> createSphere();

		std::pair<std::vector<Vertex>, std::vector<uint32_t>> createMeshObject(const std::string &name);

		void setupMeshes(const std::shared_ptr<MeshData3D>& meshData);

		// add cube, sphere, etc.
	private:
		std::unordered_map<std::string, std::function<std::pair<std::vector<Vertex>, std::vector<uint32_t>>()>> meshObjects;

		void addObjectsIntoMap();
	};
}