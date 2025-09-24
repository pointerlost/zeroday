//
// Created by pointerlost on 8/13/25.
//
#include "Editor/InspectorPanel.h"
#include "core/EngineConfig.h"
#include <imgui.h>
#include "glm/ext.hpp"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"
#include "Scene/Components.h"
#include "Graphics/OpenGL/Material/material.h"

namespace Zeroday::Editor::UI {

    void InspectorPanel::Draw(EditorState &state) {
        // const auto world = state.world;
        //
        // float inspectorX = InspectorWidth;
        // float inspectorY = InspectorHeight;
        // ImGui::SetNextWindowPos({SCR_WIDTH - InspectorWidth, MainMenuBarHeight}, ImGuiCond_Always);
        // ImGui::SetNextWindowSize({inspectorX, inspectorY}, ImGuiCond_Always);
        //
        // ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize);
        //
        // auto* nameComp         = world->GetComponent<ecs::NameComponent>(state.selectedEntity);
        // auto* transformComp= world->GetComponent<ecs::TransformComponent>(state.selectedEntity);
        // auto* matComp        = world->GetComponent<ecs::MaterialComponent>(state.selectedEntity);
        // auto* lightComp         = world->GetComponent<ecs::LightComponent>(state.selectedEntity);
        // auto* camComp        = world->GetComponent<ecs::CameraComponent>(state.selectedEntity);
        //
        // if (nameComp && deleteEntity(*nameComp, state)) {
        //     ImGui::End();
        //     return;
        // }
        // if (nameComp)      drawComponentUI(*nameComp);
        // if (transformComp) drawComponentUI(*transformComp);
        // if (camComp)       drawComponentUI(*camComp);
        // if (lightComp)     drawComponentUI(*lightComp);
        // if (matComp)       drawComponentUI(*matComp);
        //
        // ImGui::End();
    }

    void InspectorPanel::drawComponentUI(NameComponent &comp) {
        char buffer[128];
        strncpy( buffer, comp.name.c_str(), sizeof(buffer) );

        if (ImGui::CollapsingHeader("Name")) {
            ImGui::SetNextItemWidth(400);
            ImGui::InputText("##NameInput ", buffer, sizeof(buffer));
            comp.name = buffer;
        }
    }

    void InspectorPanel::drawComponentUI(TransformComponent &comp) {
        ImGui::PushID("Transform");
        auto& transform = comp.m_Transform;

        auto pos = transform.GetPosition();
        auto rot = transform.GetEulerRotation();
        auto scl = transform.GetScale();

        if (ImGui::CollapsingHeader("Transform")) {
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1)) transform.SetPosition(pos);
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1)) transform.SetRotation(rot);
            if (ImGui::DragFloat3("Scale",    glm::value_ptr(scl), 0.1, 0.05)) transform.SetScale(scl);
        }

        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(MaterialComponent &comp) {
        ImGui::PushID("Material");
        auto& material = comp.m_Instance;

        if (ImGui::CollapsingHeader("Material")) {
            auto baseColor = material->GetBaseColor();
            if (ImGui::ColorEdit4("Base Color R",  &baseColor.x,  ImGuiColorEditFlags_None)) {
                material->SetBaseColor(baseColor);
            }
        }

        float metallic = material->GetMetallic();
        if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f)) {
            material->SetMetallic(metallic);
        }

        float roughness = material->GetRoughness();
        if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
            material->SetRoughness(roughness);
        }

        glm::vec3 emissive = material->GetEmissive();
        if (ImGui::ColorEdit3("Emissive", &emissive.x)) {
            material->SetEmissive(emissive);
        }

        if (ImGui::CollapsingHeader("Texture")) {
            if (ImGui::Button("On")) {

            }
            if (ImGui::Button("Off")) {

            }
        }
        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(LightComponent &comp) {
        ImGui::PushID("Light");
        auto& light = comp.m_Light;

        // Light Type selection
        const char* lightTypes[] = { "Directional", "Point", "Spot" };
        int currentType = static_cast<int>(light.GetType());
        if (ImGui::Combo("Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
            // Handle type change if needed - you might want to create a new Light object
            // with the appropriate type and copy relevant properties
        }

        ImGui::Separator();

        // Radiance (color) with color picker
        glm::vec3 radiance = light.GetRadiance();
        float radianceArray[3] = { radiance.x, radiance.y, radiance.z };
        if (ImGui::ColorEdit3("Radiance", radianceArray)) {
            light.SetRadiance(glm::vec3(radianceArray[0], radianceArray[1], radianceArray[2]));
        }

        // Intensity
        float intensity = light.GetIntensity();
        if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f)) {
            light.SetIntensity(intensity);
        }

        ImGui::Separator();

        // Position (for point and spot lights)
        if (light.GetType() != opengl::LightType::Directional) {
            glm::vec3 position = light.GetPosition();
            float posArray[3] = { position.x, position.y, position.z };
            if (ImGui::DragFloat3("Position", posArray, 0.1f)) {
                light.SetPosition(glm::vec3(posArray[0], posArray[1], posArray[2]));
            }
        }

        // Direction (for directional and spot lights)
        if (light.GetType() != opengl::LightType::Point) {
            glm::vec3 direction = light.GetDirection();
            float dirArray[3] = { direction.x, direction.y, direction.z };
            if (ImGui::DragFloat3("Direction", dirArray, 0.01f, -1.0f, 1.0f)) {
                glm::vec3 newDir = glm::vec3(dirArray[0], dirArray[1], dirArray[2]);
                if (glm::length(newDir) > 0.001f) {
                    light.SetDirection(glm::normalize(newDir));
                }
            }
        }

        ImGui::Separator();

        // Spotlight
        if (light.GetType() == opengl::LightType::Spot) {
            // Convert cosine angles to degrees for more intuitive editing
            float cutOffDeg      = glm::degrees(glm::acos(light.GetCutOff()));
            float outerCutOffDeg = glm::degrees(glm::acos(light.GetOuterCutOff()));

            if (ImGui::DragFloat("Cut Off (degrees)", &cutOffDeg, 1.0f, 1.0f, 89.0f)) {
                light.SetCutOff(glm::cos(glm::radians(cutOffDeg)));
            }

            if (ImGui::DragFloat("Outer Cut Off (degrees)", &outerCutOffDeg, 1.0f, 1.0f, 89.0f)) {
                light.SetOuterCutOff(glm::cos(glm::radians(outerCutOffDeg)));
            }

            // Ensure outer cut off is always larger than inner cut off
            if (outerCutOffDeg <= cutOffDeg) {
                outerCutOffDeg = cutOffDeg + 1.0f;
                light.SetOuterCutOff(glm::cos(glm::radians(outerCutOffDeg)));
            }
        }

        // Attenuation parameters (for point and spot lights)
        if (light.GetType() != opengl::LightType::Directional) {
            ImGui::Separator();
            ImGui::Text("Attenuation");

            float constant = light.GetConstant();
            if (ImGui::DragFloat("Constant", &constant, 0.01f, 0.0f, 10.0f)) {
                light.SetConstant(constant);
            }

            float linear = light.GetLinear();
            if (ImGui::DragFloat("Linear", &linear, 0.001f, 0.0f, 1.0f)) {
                light.SetLinear(linear);
            }

            float quadratic = light.GetQuadratic();
            if (ImGui::DragFloat("Quadratic", &quadratic, 0.0001f, 0.0f, 0.01f, "%.6f")) {
                light.SetQuadratic(quadratic);
            }
        }

        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(CameraComponent &comp) {
        ImGui::PushID("Camera");

        if (ImGui::CollapsingHeader("View")) {
            auto& camera = comp.m_Camera;

            float fov  = camera.GetPerspectiveFOV();
            float near = camera.GetPerspectiveNear();
            float far  = camera.GetPerspectiveFar();

            ImGui::DragFloat("Fov",  &fov,  0.1f, 0.0f, 8000.0f);
            ImGui::DragFloat("Near", &near, 0.5f, 0.0f, 0.8f);
            ImGui::DragFloat("Far",  &far,  1.5f, 0.0f, 10000.0f);

            camera.SetProjectionMatrix(fov, near, far);
        }

        ImGui::PopID();
    }

    bool InspectorPanel::deleteEntity(NameComponent &comp, EditorState &state) {
        ImGui::PushID("Delete");
        static bool deleteWindowOpen = false;
        bool deleted = false;

        if (ImGui::Button("Delete Entity", ImVec2(InspectorWidth - 15, 50))) {
            deleteWindowOpen = true;
        }

        if (deleteWindowOpen) {
            ImGui::SetNextWindowSize(ImVec2(350, 150));
            ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH / 2, SCR_HEIGHT / 2));

            if (ImGui::Begin("Delete Entity", &deleteWindowOpen, ImGuiWindowFlags_NoResize)) {
                ImGui::Text("Are you sure you want to delete the Entity?");
                // add some space
                ImGui::Dummy(ImVec2(0, 20));

                if (ImGui::Button("Yes", ImVec2(162, 50))) {
                    // deleteWindowOpen = false;
                    // if (state.cameraEntity != state.selectedEntity) {
                    //     deleted = true;
                    //     state.world->RemoveEntity(state.selectedEntity);
                    //     state.selectedEntity = -1;
                    // }
                }

                ImGui::SameLine();

                if (ImGui::Button("No", ImVec2(162, 50))) {
                    deleteWindowOpen = false;
                }
                ImGui::End();
            }
        }
        ImGui::PopID();
        return deleted;
    }
}
