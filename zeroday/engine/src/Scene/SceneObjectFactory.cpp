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
	SceneObjectFactory::SceneObjectFactory(ecs::Scene& w) : world(w) {}

    ecs::Entity SceneObjectFactory::CreatePrimitiveObject(const std::string& meshName) const
    {
		auto* matLib = Services::GetMaterialLibrary();
		auto* meshLib  = Services::GetMeshLibrary();

		ecs::NameComponent nc;

    	const ecs::Entity entity = world.CreateEntity();
    	const auto& meshData = meshLib->GetMeshData3D();

		ecs::TransformComponent tc;
		tc.transform = CreateRef<Transform>();

		ecs::MaterialComponent matC;
		matC.instance = matLib->CreateInstance("wood");

	    ecs::MeshComponent meshC;
    	// create default mesh properties
    	meshC.meshData    = meshData;
    	meshC.subMeshName = meshName;

		world.AddComponent(entity, std::move(nc));
		world.AddComponent(entity, std::move(tc));
    	world.AddComponent(entity, std::move(matC));
    	world.AddComponent(entity, std::move(meshC));

    	return entity;
    }

    ecs::Entity SceneObjectFactory::CreateLight(LIGHTING::LightType type, const std::string& meshName) const
	{
		auto* matLib = Services::GetMaterialLibrary();
		auto* meshLib  = Services::GetMeshLibrary();

    	const ecs::Entity entity = world.CreateEntity();
    	const auto& meshData = meshLib->GetMeshData3D();

		ecs::NameComponent nc;

		ecs::TransformComponent tc;
		tc.transform = CreateRef<Transform>();

		ecs::MaterialComponent matC;
		matC.instance->base = matLib->GetMaterialByName("lightSource");

		ecs::LightComponent lc;
		lc.light = CreateRef<LIGHTING::Light>(type);

    	// create default mesh properties
		ecs::MeshComponent meshC;
    	meshC.meshData = meshData;
    	meshC.subMeshName = meshName;

		world.AddComponent(entity, std::move(nc));
		world.AddComponent(entity, std::move(tc));
    	world.AddComponent(entity, std::move(meshC));
    	world.AddComponent(entity, std::move(matC));
		world.AddComponent(entity, std::move(lc));

    	return entity;
    }

    ecs::Entity SceneObjectFactory::CreateCamera(ecs::CameraType type) {
		ecs::Entity entity = world.CreateEntity();

		ecs::NameComponent nComp;

		ecs::TransformComponent tComp;
		tComp.transform = CreateRef<Transform>();

		ecs::CameraComponent cameraComp(type);

		world.AddComponent(entity, std::move(tComp));
		world.AddComponent(entity, std::move(nComp));
		world.AddComponent(entity, cameraComp);

		return entity;
    }

    ecs::Entity SceneObjectFactory::CreateModel(const std::string &path) const {
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
