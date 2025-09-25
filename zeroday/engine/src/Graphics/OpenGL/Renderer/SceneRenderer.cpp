//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Renderer/RenderCommand.h"

namespace Zeroday::opengl {

    ExtractResult SceneRenderer::ExtractRenderables(Scene *scene) {
        ExtractResult result;

        const auto view = scene->GetAllEntitiesWith<
            TransformComponent,
            MeshComponent,
            MaterialComponent
        >();

        for (auto [entity, transform, mesh, material] : view.each()) {
            ExtractResult renderable;

            TransformSSBO transformData;
            glm::mat4 modelMatrix = transform.m_Transform.GetModelMatrix();
            transformData.modelMatrix = modelMatrix;
            transformData.normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
            result.transforms.push_back(transformData);
            uint32_t transformIndex = result.transforms.size() - 1;

            MaterialSSBO materialData = material.m_Instance ?
                material.m_Instance->ToGPUFormat() : MaterialSSBO{};
            result.materials.push_back(materialData);
            uint32_t materialIndex = result.materials.size() - 1;

            // get mesh info for renderable entity
            const auto& meshInfo = mesh.meshData->getMeshInfo(mesh.subMeshName);

            // Fill command for per-entity
            RenderCommandMDI cmd;
            cmd.materialIndex  = materialIndex;
            cmd.transformIndex = transformIndex;
            cmd.indexCount     = meshInfo.m_IndexCount;
            cmd.indexOffset    = meshInfo.m_IndexOffset;
            result.renderCommands.push_back(cmd);
        }

        // Extract lights to upload GPU
        ExtractLights(scene, result);
        // Extract cameras to upload GPU
        ExtractCamera(scene, result);

        return result;
    }

    void SceneRenderer::ExtractLights(Scene* scene, ExtractResult& result) {
        auto view = scene->GetAllEntitiesWith<LightComponent, TransformComponent>();
        for (auto [entity, lightComp, transform] : view.each()) {
            LightSSBO gpuLight = lightComp.ToGPUFormat();
            gpuLight.position = transform.m_Transform.GetPosition();

            if (lightComp.m_Light.GetType() != LightType::Point) {
                gpuLight.direction = transform.m_Transform.GetForward();
            }

            result.lights.push_back(gpuLight);
        }
    }

    void SceneRenderer::ExtractCamera(Scene *scene, ExtractResult &result) {
        auto view = scene->GetAllEntitiesWith<CameraComponent, TransformComponent>();

        for (auto [entity, cameraComp, transform] : view.each()) {
            CameraUBO camera;
            camera.position  = transform.m_Transform.GetPosition();
            camera.direction = transform.m_Transform.GetForward();
            camera.view      = cameraComp.m_Camera.GetViewMatrix();
            camera.projection     = cameraComp.m_Camera.GetProjectionMatrix();
            camera.viewProjection = camera.projection * camera.view;

            result.camera = camera;
        }
    }
}
