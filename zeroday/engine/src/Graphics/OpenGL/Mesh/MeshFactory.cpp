#include "Graphics/OpenGL/Mesh/MeshFactory.h"
#include <Graphics/OpenGL/Mesh/MeshData3D.h>
#include "Core/Logger.h"
#include "Math/Math.h"
#include "Core/Base.h"

namespace Zeroday
{
	MeshFactory::MeshFactory() {
		AddObjectsIntoMap();
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateTriangle() {
		std::vector<Vertex> vertices = {
			{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.5f, 1.0f) }
		};

		std::vector<uint32_t> indices = { 0, 1, 2 };

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateSquare() {
		std::vector<Vertex> vertices = {
			{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
			{ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
			{ glm::vec3( 0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
			{ glm::vec3(-0.5f,  0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f) }
		};

		std::vector<uint32_t> indices = { 0, 1, 2, 2, 3, 0 };

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateCube() {
		std::vector<Vertex> vertices =
		{	// Vertices						                       Normal						  Color						     TexCoords
			
			// Front face (+Z)
			{ glm::vec3(-0.5f, -0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,   0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f,  0.5f, 0.5f),   glm::vec3 {0.0,  0.0, 1.0},  {0.0f, 1.0f}  },

			// Back face  (-Z)
			{ glm::vec3(0.5f,  -0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0}, {0.0f, 0.0f}  },
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0}, {1.0f, 0.0f}  },
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0}, {1.0f, 1.0f}  },
			{ glm::vec3(0.5f,   0.5f, -0.5f),  glm::vec3 {0.0,  0.0, -1.0}, {0.0f, 1.0f}  },

			// Left face  (-X)
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {-1.0,  0.0, 0.0}, {0.0f, 0.0f}  },
			{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3 {-1.0,  0.0, 0.0}, {1.0f, 0.0f}  },
			{ glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3 {-1.0,  0.0, 0.0}, {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3 {-1.0,  0.0, 0.0}, {0.0f, 1.0f}  },

			// Right face (+X)
			{ glm::vec3(0.5f, -0.5f,  0.5f),   glm::vec3 {1.0,  0.0, 0.0},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f, -0.5f, -0.5f),   glm::vec3 {1.0,  0.0, 0.0},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3 {1.0,  0.0, 0.0},  {1.0f, 1.0f}  },
			{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3 {1.0,  0.0, 0.0},  {0.0f, 1.0f}  },

			// Top face (+Y)
			{ glm::vec3(-0.5f, 0.5f,  0.5f),   glm::vec3 {0.0,  1.0, 0.0},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f,  0.5f),   glm::vec3 {0.0,  1.0, 0.0},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  0.5f, -0.5f),   glm::vec3 {0.0,  1.0, 0.0},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f, 0.5f, -0.5f),   glm::vec3 {0.0,  1.0, 0.0},  {0.0f, 1.0f}  },

			// Bottom face (-Y)
			{ glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3 {0.0, -1.0, 0.0},  {0.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f, -0.5f),  glm::vec3 {0.0, -1.0, 0.0},  {1.0f, 0.0f}  },
			{ glm::vec3(0.5f,  -0.5f,  0.5f),  glm::vec3 {0.0, -1.0, 0.0},  {1.0f, 1.0f}  },
			{ glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3 {0.0, -1.0, 0.0},  {0.0f, 1.0f}  },
		};

		std::vector<uint32_t> indices =
		{
			// Front face
			0, 1, 2,      2, 3, 0,

			// Back face
			4, 5, 6,      6, 7, 4,

			// Left face
			8, 9, 10,     10, 11, 8,

			// Right face
			12, 13, 14,   14, 15, 12,

			// Top face
			16, 17, 18,   18, 19, 16,

			// Bottom face
			20, 21, 22,   22, 23, 20,
		};

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateCircle() {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		constexpr int segmentCount = 30;
		constexpr float radius = 1.0f;

		constexpr auto centerTexCoord = glm::vec2(0.5f, 0.5f);
		constexpr auto normal = glm::vec3(0.0f, 0.0f, 1.0f);

		vertices.push_back({ glm::vec3(0.0f,0.0f,0.0f), normal, centerTexCoord }); // center vertex must be first

		for (int i = 0; i < segmentCount; i++) {

			float angle = 2.0f * M_PI * i / segmentCount;
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			float z = 0.0f;

			const auto position = glm::vec3(x, y, z);
			const auto uv = glm::vec2((x + 1.0f) * 0.5f, (y + 1.0f) * 0.5f); // for texture normalizing

			vertices.push_back({ position, normal, uv });
		}

		for (int i = 1; i <= segmentCount; i++) {
			indices.push_back(0); // center vertex
			indices.push_back(i);
			if (i == segmentCount)
				indices.push_back(1); // wrap around
			else
				indices.push_back(i + 1);
		}

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateSphere() {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;

		constexpr uint xSegments = 15;
		constexpr uint ySegments = 15;

		for (unsigned int y = 0; y <= ySegments; ++y) {
			for (unsigned int x = 0; x <= xSegments; ++x) {
				constexpr float radius = 0.2f;
				const float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
				const float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
				const float xPos = std::cos(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI) * radius;
				const float yPos = std::cos(ySegment * M_PI) * radius;
				const float zPos = std::sin(xSegment * 2.0f * M_PI) * std::sin(ySegment * M_PI) * radius;

				const glm::vec3 pos = glm::vec3(xPos, yPos, zPos);
				const glm::vec3 normal = glm::normalize(glm::vec3(xPos, yPos, zPos));

				vertices.push_back({
					// where the multiplied numbers are the radius of the sphere
					pos,	// pos
					normal, // normal
					glm::vec2(xSegment, ySegment) // UV
				});
			}
		}

		for (unsigned int y = 0; y < ySegments; ++y) {
			for (unsigned int x = 0; x < xSegments; ++x) {
				uint32_t i0 = y * (xSegments + 1) + x;
				uint32_t i1 = i0 + xSegments + 1;

				indices.push_back(i0);
				indices.push_back(i1);
				indices.push_back(i0 + 1);

				indices.push_back(i0 + 1);
				indices.push_back(i1);
				indices.push_back(i1 + 1);
			}
		}

		return std::make_pair(vertices, indices);
	}

	std::pair<std::vector<Vertex>, std::vector<uint32_t>> MeshFactory::CreateMeshObject(const std::string &name) {
		if (const auto it = meshObjects.find(name); it != meshObjects.end()) {
			return it->second();
		}
		Warn("MeshFactory: Mesh object with name '" + name + "' not found.");
		return{};
	}

	void MeshFactory::AddObjectsIntoMap() {
		meshObjects["triangle"] = CreateTriangle;
		meshObjects["square"]   = CreateSquare;
		meshObjects["cube"]     = CreateCube;
		meshObjects["circle"]   = CreateCircle;
		meshObjects["sphere"]   = CreateSphere;
	}
}
