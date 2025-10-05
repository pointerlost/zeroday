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

		static std::pair<std::vector<Vertex>, std::vector<uint32_t>> CreateMeshObject(const std::string &name);

		// add cube, sphere, etc.
	private:
		static inline std::unordered_map<std::string, std::function<std::pair<std::vector<Vertex>, std::vector<uint32_t>>()>> meshObjects;

		static void AddObjectsIntoMap();
	};
}