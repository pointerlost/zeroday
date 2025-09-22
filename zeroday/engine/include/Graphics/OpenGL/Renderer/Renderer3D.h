#pragma once
#include <iostream>
#include <glm/ext.hpp>

namespace Zeroday {
	namespace Ecs { struct MaterialComponent; }
	struct MeshData3D;
	class Camera;
	class Scene;
}


namespace Zeroday::opengl
{
	class Renderer3D
	{
	public:
		explicit Renderer3D(Scene* scene) : m_Scene(scene) {}

		void BeginScene(const Camera& camera, const glm::mat4& viewMatrix);
		void EndScene();

		void Render();

		void Submit(
			const MeshData3D& meshData,
			const std::string& subMeshName,
			const glm::mat4& transform,
			const Ecs::MaterialComponent& material
		);

	private:
		Scene* m_Scene = nullptr;
	};
}
