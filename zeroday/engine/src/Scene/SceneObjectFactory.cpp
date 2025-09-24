#include "Scene/SceneObjectFactory.h"
#include "Scene/Scene.h"
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Model/Model.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"

namespace Zeroday
{
	SceneObjectFactory::SceneObjectFactory(Scene& scene) : m_Scene(scene) {}

    Entity SceneObjectFactory::CreatePrimitiveObject(const std::string& meshName) const
    {
		// auto* matLib = Services::GetMaterialLibrary();
		// auto* meshLib  = Services::GetMeshLibrary();
	 //
		// NameComponent nc;
	 //
  //   	const Entity entity = world.CreateEntity();
  //   	const auto& meshData = meshLib->GetMeshData3D();
	 //
		// TransformComponent tc;
		// tc.m_Transform = CreateRef<Transform>();
	 //
		// MaterialComponent matC;
		// matC.m_Instance = matLib->CreateInstance("wood");
	 //
	 //    MeshComponent meshC;
  //   	// create default mesh properties
  //   	meshC.meshData    = meshData;
  //   	meshC.subMeshName = meshName;
	 //
		// world.AddComponent(entity, std::move(nc));
		// world.AddComponent(entity, std::move(tc));
  //   	world.AddComponent(entity, std::move(matC));
  //   	world.AddComponent(entity, std::move(meshC));
	 //
  //   	return entity;
    }

    Entity SceneObjectFactory::CreateLight(opengl::LightType type, const std::string& meshName) const {
		// auto* matLib = Services::GetMaterialLibrary();
		// auto* meshLib  = Services::GetMeshLibrary();
	 //
  //   	Entity entity = world.CreateEntity();
  //   	const auto& meshData = meshLib->GetMeshData3D();
	 //
		// NameComponent nc;
	 //
		// TransformComponent tc;
		// tc.m_Transform = Transform{};
	 //
		// MaterialComponent matC;
		// matC.m_Instance->m_Base = matLib->GetMaterialByName("lightSource");
	 //
		// LightComponent lc;
		// lc.m_Light = opengl::Light{};
	 //
  //   	// create default mesh properties
		// MeshComponent meshC{};
  //   	meshC.meshData = meshData;
  //   	meshC.subMeshName = meshName;
	 //
		// world.AddComponent(entity, std::move(nc));
		// world.AddComponent(entity, std::move(tc));
  //   	world.AddComponent(entity, std::move(meshC));
  //   	world.AddComponent(entity, std::move(matC));
		// world.AddComponent(entity, std::move(lc));
	 //
  //   	return entity;
    }

    Entity SceneObjectFactory::CreateCamera(CameraMode mode) {
		// Entity entity = scene.CreateEntity();

		// NameComponent nComp;

		// TransformComponent tComp;
		// tComp.m_Transform = Transform{};

		// CameraComponent cameraComp{};

		// world.AddComponent(entity, std::move(tComp));
		// world.AddComponent(entity, std::move(nComp));
		// world.AddComponent(entity, cameraComp);

		// return entity;
    }

    Entity SceneObjectFactory::CreateModel(const std::string &path) const {
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
