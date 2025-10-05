#include "Scene/SceneObjectFactory.h"

#include <Core/Config.h>
#include <nlohmann/detail/string_concat.hpp>

#include "Core/AssetManager.h"
#include "Core/File.h"
#include "Core/Services.h"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Model/Model.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"

namespace Zeroday {

	SceneObjectFactory::SceneObjectFactory(Scene& scene) : m_Scene(scene) {}

    Entity SceneObjectFactory::CreatePrimitiveObject(const std::string& meshName, const std::string& entityName) const {
		auto entity = m_Scene.CreateEntity(entityName);

		auto mat = Services::GetAssetManager()->CreateMaterialInstance("metal_048a");
		entity.AddComponent<MaterialComponent>(mat);

        MeshComponent meshComp{};
        meshComp.m_SubMeshName = meshName;
        entity.AddComponent<MeshComponent>(meshComp);

        return entity;
    }

    Entity SceneObjectFactory::CreateLight(opengl::LightType type,
										const std::string& entityName, const std::string& meshName) const {
		auto entity = m_Scene.CreateEntity(entityName);

		auto mat = Services::GetAssetManager()->CreateMaterialInstance("onyx_015");
		entity.AddComponent<MaterialComponent>(mat);

		LightComponent lightComp{type};
		entity.AddComponent<LightComponent>(lightComp);

		MeshComponent meshComp{};
		meshComp.m_SubMeshName = meshName;
		entity.AddComponent<MeshComponent>(meshComp);

		// Add transform component
		if (!entity.HasComponent<TransformComponent>()) {
			entity.AddComponent<TransformComponent>();
		}

		return entity;
    }

    Entity SceneObjectFactory::CreateCamera(CameraMode mode, const std::string& entityName) {

		auto entity = m_Scene.CreateEntity(entityName);

		CameraComponent cameraComp{mode};
		entity.AddComponent<CameraComponent>(cameraComp);
		entity.GetComponent<TransformComponent>().m_Transform.SetPosition(glm::vec3(0.0, 0.0, 5.0));

		return entity;
    }

	Entity SceneObjectFactory::TestModelLoader(const std::string& path, const std::string& entityName) const {
		const auto modelLoader = CreateScope<Graphics::ModelLoader>();

		if (!File::Get().Exists(path)) {
			Warn("Model not found: " + path);
			return Entity{};
		}

		auto model = modelLoader->Load(path);

		if (model && model->IsValid()) {
			Info(std::format(" - Name: {}", model->m_Name));
			Info(std::format(" - Meshes: {}", model->GetMeshCount()));

			// Create a single entity with ModelComponent
			auto entity = m_Scene.CreateEntity(model->m_Name);

			// Add ModelComponent that references the entire model
			entity.AddComponent<ModelComponent>(model);

			// Add transform
			auto& transform = entity.GetComponent<TransformComponent>();
			transform.m_Transform.SetScale(glm::vec3(0.5f));

			return entity;
		} else {
			Error("Failed to load Model!");
		}

		return Entity{};
	}
}
