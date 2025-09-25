#include "Scene/SceneObjectFactory.h"
#include "Scene/Scene.h"
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Model/Model.h"

namespace Zeroday
{
	SceneObjectFactory::SceneObjectFactory(Scene& scene) : m_Scene(scene) {}

    Entity SceneObjectFactory::CreatePrimitiveObject(const std::string& meshName, const std::string& entityName) const
    {
        auto* matLib = Services::GetMaterialLibrary();
        const auto* meshLib = Services::GetMeshLibrary();

		auto entity = m_Scene.CreateEntity(entityName);

		auto mat = matLib->CreateInstance("wood");
		entity.AddComponent<MaterialComponent>(mat);

        MeshComponent meshComp{};
        meshComp.meshData = meshLib->GetMeshData3D();
        meshComp.subMeshName = meshName;
        entity.AddComponent<MeshComponent>(meshComp);

        return entity;
    }

    Entity SceneObjectFactory::CreateLight(opengl::LightType type,
										const std::string& entityName, const std::string& meshName) const {

		auto* matLib = Services::GetMaterialLibrary();
		auto* meshLib = Services::GetMeshLibrary();

		auto entity = m_Scene.CreateEntity(entityName);

		auto mat = matLib->CreateInstance("wood");
		entity.AddComponent<MaterialComponent>(mat);

		LightComponent lightComp{type};
		entity.AddComponent<LightComponent>(lightComp);

		MeshComponent meshComp{};
		meshComp.meshData = meshLib->GetMeshData3D();
		meshComp.subMeshName = meshName;
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
		entity.GetComponent<TransformComponent>().m_Transform.SetPosition(glm::vec3(0.0, 0.0, -5.0));

		return entity;
    }

    Entity SceneObjectFactory::CreateModel(const std::string &path, const std::string& entityName) const {
		// auto* modelLoader   = Services::GetModelLoader();
		// auto* matLib        = Services::GetMaterialLibrary();

		// const Scene::Entity entity = world.CreateEntity();
		// auto model = modelLoader->load(path);
		// if (!model) {
		// 	Logger::warn("[SceneObjectFactory::createModelWithPath] Failed to load model " + path);
		// 	return entity;
		// }
		//
		// for (auto& meshEntry : model->meshes) {
		// 	const Scene::MaterialComponent matC;
		// 	if (meshEntry.subMeshName.empty()) {
		// 		matC.instance->base = matLib->getDefaultMaterial();
		// 	} else {
		// 		matC.instance->base = matLib->getMaterialByName(meshEntry.subMeshName);
		// 	}
		// }
		//
		// Scene::TransformComponent tComp;
		// tComp.transform = CreateRef<Transform>();
		//
		// Scene::ModelComponent modelComponent;
		// modelComponent.model = std::move(model);
		//
		// world.AddComponent(entity, std::move(tComp));
		// world.AddComponent(entity, std::move(modelComponent));

		// return entity;
    }
}
