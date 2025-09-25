#pragma once
#include <memory>
#include <unordered_map>
#include <functional>

namespace Zeroday
{
	struct Vertex;
	struct MeshData3D;

	class MeshFactory
	{
	public:
		MeshFactory();

		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateTriangle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateSquare();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateCube();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateCircle();
		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateSphere();

		std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateMeshObject(const std::string &name);

		void SetupMeshes(const std::shared_ptr<MeshData3D>& meshData);

		// add cube, sphere, etc.
	private:
		std::unordered_map<std::string, std::function<std::pair<std::vector<Vertex>, std::vector<uint32_t>>()>> meshObjects;

		void AddObjectsIntoMap();
	};
}