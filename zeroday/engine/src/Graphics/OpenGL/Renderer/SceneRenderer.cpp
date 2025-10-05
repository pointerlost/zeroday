//
// Created by pointerlost on 9/23/25.
//
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"
#include "Core/AssetManager.h"
#include "Core/Services.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/GPU_buffers.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Model/Model.h"
#include "Graphics/OpenGL/Renderer/RenderCommand.h"

namespace Zeroday::opengl {

    ExtractResult SceneRenderer::ExtractRenderables(Scene *scene) {
        ExtractResult result;
        const auto view = scene->GetAllEntitiesWith<TransformComponent, MeshComponent, MaterialComponent>();

        // Track which entities we've processed to avoid duplicates
        std::set<entt::entity> processedEntities;

        for (auto [entity, transform, mesh, material] : view.each()) {
            // Skip if already processed (in case entity has multiple relevant components)
            if (processedEntities.contains(entity)) {
                continue;
            }
            processedEntities.insert(entity);

            TransformSSBO transformData;
            glm::mat4 modelMatrix = transform.m_Transform.GetWorldMatrix();
            transformData.modelMatrix = modelMatrix;
            transformData.normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
            result.transforms.push_back(transformData);
            uint32_t transformIndex = result.transforms.size() - 1;

            MaterialSSBO materialData = material.m_Instance
                ? material.m_Instance->ToGPUFormat()
                : MaterialSSBO{};
            result.materials.push_back(materialData);
            uint32_t materialIndex = result.materials.size() - 1;

            const auto& meshInfo = Services::GetMeshLibrary()->GetMeshData3D()->GetMeshInfo(mesh.m_SubMeshName);

            RenderCommandMDI cmd;
            cmd.materialIndex  = materialIndex;
            cmd.transformIndex = transformIndex;
            cmd.indexCount     = meshInfo.m_IndexCount;
            cmd.indexOffset    = meshInfo.m_IndexOffset;
            result.renderCommands.push_back(cmd);
        }

        ExtractLights(scene, result);
        ExtractCamera(scene, result);
        ExtractGlobalData(scene, result);
        ExtractModels(scene, result);

        return result;
    }

    void SceneRenderer::ExtractLights(Scene* scene, ExtractResult& result) {
        auto view = scene->GetAllEntitiesWith<LightComponent, TransformComponent>();
        for (auto [entity, lightComp, transform] : view.each()) {
            LightSSBO gpuLight = lightComp.ToGPUFormat(transform);
            result.lights.push_back(gpuLight);
        }
    }

    void SceneRenderer::ExtractCamera(Scene *scene, ExtractResult &result) {
        auto view = scene->GetAllEntitiesWith<CameraComponent, TransformComponent>();

        for (auto [entity, cameraComp, transform] : view.each()) {
            CameraUBO camera;
            camera.position       = glm::vec4(transform.m_Transform.GetPosition(), 1.0f);
            camera.direction      = glm::vec4(transform.m_Transform.GetForward(), 0.0f);

            cameraComp.m_Camera.UpdateViewMatrix(transform.m_Transform.GetWorldMatrix());

            camera.view           = cameraComp.m_Camera.GetViewMatrix();
            camera.projection     = cameraComp.m_Camera.GetProjectionMatrix();
            camera.viewProjection = cameraComp.m_Camera.GetProjectionViewMatrix();

            result.camera = camera;
            break;
        }
    }

    void SceneRenderer::ExtractGlobalData(Scene *scene, ExtractResult &result) {
        GlobalUBO global;
        global.globalAmbient = g_GlobalAmbient;
        result.globalData = global;
    }

    void SceneRenderer::ExtractModels(Scene *scene, ExtractResult &result) {
        auto view = scene->GetAllEntitiesWith<TransformComponent, ModelComponent>();

        for (auto [entity, transform, modelComp] : view.each()) {
            if (!modelComp.model || !modelComp.model->IsValid()) continue;

            TransformSSBO transformData;
            glm::mat4 modelMatrix = transform.m_Transform.GetWorldMatrix();
            transformData.modelMatrix = modelMatrix;
            transformData.normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
            uint32_t transformIndex = result.transforms.size();
            result.transforms.push_back(transformData);

            // Process ALL meshes in the model
            for (const auto& meshEntry : modelComp.model->m_Meshes) {
                // Get mesh data from library
                auto meshData = Services::GetMeshLibrary()->GetMeshData3D();
                if (!meshData) {
                    Warn("Mesh not found in library: " + meshEntry.m_SubMeshName);
                    continue;
                }

                // Get submesh info (assuming first submesh for now)
                const auto& meshInfo = meshData->GetMeshInfo(meshEntry.m_SubMeshName);

                MaterialSSBO materialData = meshEntry.m_Material
                    ? meshEntry.m_Material->ToGPUFormat()
                    : MaterialSSBO{};
                result.materials.push_back(materialData);
                uint32_t materialIndex = result.materials.size() - 1;

                RenderCommandMDI cmd;
                cmd.materialIndex = materialIndex;
                cmd.transformIndex = transformIndex; // Same transform for all meshes in model
                cmd.indexCount = meshInfo.m_IndexCount;
                cmd.indexOffset = meshInfo.m_IndexOffset;
                result.renderCommands.push_back(cmd);
            }
        }
    }
}
