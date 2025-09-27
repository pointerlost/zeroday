//
// Created by pointerlost on 8/13/25.
//
#include "Editor/InspectorPanel.h"
#include "core/EngineConfig.h"
#include <imgui.h>
#include "Editor/EditorState.h"
#include "glm/ext.hpp"
#include "Scene/Scene.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"
#include "Scene/Components.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Scene/Entity.h"

namespace Zeroday::Editor::UI {

    void InspectorPanel::Draw(EditorState &state) {
        auto scene = state.scene;

        float inspectorX = InspectorWidth;
        float inspectorY = InspectorHeight;
        ImGui::SetNextWindowPos({SCR_WIDTH - InspectorWidth, MainMenuBarHeight}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({inspectorX, inspectorY}, ImGuiCond_Always);

        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize);

        if (!state.selectedEntity.IsValid()) {
            ImGui::End();
            return;
        }

        Entity entity = scene->GetEntityWithUUID(state.selectedEntity.GetUUID());

        auto [nameComp, transformComp, materialComp, lightComp, cameraComp]
        = entity.TryGetAllComponents<NameComponent, TransformComponent, MaterialComponent, LightComponent, CameraComponent>();

        DrawComponentUI(*nameComp);
        DrawComponentUI(*transformComp);
        if (materialComp) {
            DrawComponentUI(*materialComp);
        }
        if (entity.HasComponent<CameraComponent>()) {
            DrawComponentUI(*cameraComp);
        }
        if (entity.HasComponent<LightComponent>()) {
            DrawComponentUI(*lightComp);
        }
        ImGui::End();
    }

    void InspectorPanel::DrawComponentUI(NameComponent &comp) {
        char buffer[128];
        strncpy( buffer, comp.name.c_str(), sizeof(buffer) );

        if (ImGui::CollapsingHeader("Name")) {
            ImGui::SetNextItemWidth(400);
            ImGui::InputText("##NameInput ", buffer, sizeof(buffer));
            comp.name = buffer;
        }
    }

    void InspectorPanel::DrawComponentUI(TransformComponent &comp) {
        ImGui::PushID("Transform");
        auto& transform = comp.m_Transform;

        auto pos = transform.GetPosition();
        auto rot = transform.GetEulerRotation();
        auto scl = transform.GetScale();

        if (ImGui::CollapsingHeader("Transform")) {
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1)) {
                transform.SetPosition(pos);
            }
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1)) {
                transform.SetEulerRotation(rot);
            }
            if (ImGui::DragFloat3("Scale",    glm::value_ptr(scl), 0.1, 0.05)) {
                transform.SetScale(scl);
            }
        }

        ImGui::PopID();
    }

    void InspectorPanel::DrawComponentUI(MaterialComponent &comp) {
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

    void InspectorPanel::DrawComponentUI(LightComponent &comp) {
        ImGui::PushID("Light");
        auto& light = comp.m_Light;

        // Light Type selection
        const char* lightTypes[] = { "Directional", "Point", "Spot" };
        int currentType = static_cast<int>(light.GetType());
        if (ImGui::Combo("Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
            if (currentType == static_cast<int>(opengl::LightType::Point)) {
                comp.SetAsPoint(light.GetPosition(), light.GetRadiance(), light.GetIntensity());
            }
            else if (currentType == static_cast<int>(opengl::LightType::Directional)) {
                comp.SetAsDirectional(light.GetDirection(), light.GetRadiance(), light.GetIntensity());
            }
            else {
                comp.SetAsSpot(light.GetPosition(), light.GetDirection(), light.GetRadiance(), light.GetIntensity(), light.GetCutOff(), light.GetOuterCutOff());
            }
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

    void InspectorPanel::DrawComponentUI(CameraComponent &comp) {
        ImGui::PushID("Camera");

        if (ImGui::CollapsingHeader("View")) {
            auto& camera = comp.m_Camera;

            float fov = glm::degrees(camera.GetPerspectiveFOV()); // convert to degrees for UI
            float near = camera.GetPerspectiveNear();
            float far  = camera.GetPerspectiveFar();

            ImGui::DragFloat("Fov",  &fov, 0.1f, 1.0f, 179.0f); // reasonable FOV range
            ImGui::DragFloat("Near", &near, 0.5f, 0.00001f, 0.8f);
            ImGui::DragFloat("Far",  &far,  1.5f, 0.00001f, 10000.0f);

            // convert to radians again (for fov)
            camera.SetProjectionMatrix(glm::radians(fov), near, far);
        }

        ImGui::PopID();
    }

}
