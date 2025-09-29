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
#include "Graphics/OpenGL/Transformations/Transformations.h"
#include "Scene/Components.h"
#include "Graphics/OpenGL/Material/material.h"
#include "Graphics/OpenGL/Renderer/Renderer3D.h"
#include "Graphics/OpenGL/Renderer/RenderState.h"
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

        auto [tagComp, transformComp, materialComp, lightComp, cameraComp]
        = entity.TryGetAllComponents<TagComponent, TransformComponent, MaterialComponent, LightComponent, CameraComponent>();

        DrawComponentUI(*tagComp);
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

        // General Settings
        DrawComponentUI(state);

        ImGui::End();
    }

    void InspectorPanel::DrawComponentUI(TagComponent &comp) {
        char buffer[128];
        strncpy( buffer, comp.tag.c_str(), sizeof(buffer) );

        static bool headerOpened = true;
        ImGui::SetNextItemOpen(headerOpened, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Tag")) {
            ImGui::SetNextItemWidth(400);
            ImGui::InputText("##TagInput ", buffer, sizeof(buffer));
            comp.tag = buffer;
        }
    }

    void InspectorPanel::DrawComponentUI(TransformComponent &comp) {
        ImGui::PushID("Transform");
        auto& transform = comp.m_Transform;

        auto pos = transform.GetPosition();
        auto rot = transform.GetEulerRotation();
        auto scl = transform.GetScale();

        static bool headerOpened = true;
        ImGui::SetNextItemOpen(headerOpened, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Transform")) {
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1)) {
                transform.SetPosition(pos);
            }
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1, -360.0, 360.0)) {
                transform.SetEulerRotation(rot);
            }
            if (ImGui::DragFloat3("Scale",    glm::value_ptr(scl), 0.1, 0.1)) {
                transform.SetScale(scl);
            }
        }

        ImGui::PopID();
    }

    void InspectorPanel::DrawComponentUI(MaterialComponent &comp) {
        ImGui::PushID("Material");
        auto& material = comp.m_Instance;

        static bool headerOpenedMat = true;
        ImGui::SetNextItemOpen(headerOpenedMat, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Material")) {
            auto baseColor = material->GetBaseColor();
            if (ImGui::ColorEdit4("Color",  &baseColor.x,  ImGuiColorEditFlags_None)) {
                material->SetBaseColor(baseColor);
            }
        }

        float metallic = material->GetMetallic();
        if (ImGui::SliderFloat("Metallic", &metallic, 0.0001f, 1.0f)) {
            material->SetMetallic(metallic);
        }

        float roughness = material->GetRoughness();
        if (ImGui::SliderFloat("Roughness", &roughness, 0.0001f, 1.0f)) {
            material->SetRoughness(roughness);
        }

        glm::vec3 emissive = material->GetEmissive();
        if (ImGui::ColorEdit3("Emissive", &emissive.x)) {
            material->SetEmissive(emissive);
        }

        static bool headerOpenedTex = true;
        ImGui::SetNextItemOpen(headerOpenedTex, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Texture")) {
            auto xButtons = InspectorWidth / 3 - 10;
            if (ImGui::Button("On", ImVec2(xButtons, 32))) {

            }
            ImGui::SameLine();
            if (ImGui::Button("Off", ImVec2(xButtons, 32))) {

            }
            ImGui::SameLine();
            if (ImGui::Button("Load", ImVec2(xButtons, 32))) {

            }
        }
        ImGui::PopID();
    }

    void InspectorPanel::DrawComponentUI(LightComponent &comp) {
        ImGui::PushID("Light");
        auto& light = comp.m_Light;

        static bool headerOpened = true;
        ImGui::SetNextItemOpen(headerOpened, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("Light Properties")) {
            // Light Type selection
            const char* lightTypes[] = { "Directional", "Point", "Spot" };
            int currentType = static_cast<int>(light.GetType());
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.3f, 0.6f, 0.9f, 0.8f));
            ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(0.2f, 0.5f, 0.8f, 0.6f));
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
            ImGui::PopStyleColor(2);

            ImGui::Separator();

            // Radiance with color picker
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
                    outerCutOffDeg = cutOffDeg + 0.25f;
                    light.SetOuterCutOff(glm::cos(glm::radians(outerCutOffDeg)));
                }
            }

            // Attenuation parameters (for point and spot lights)
            if (light.GetType() != opengl::LightType::Directional) {
                ImGui::Separator();
                ImGui::Text("Attenuation Properties");

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
        }

        ImGui::PopID();
    }

    void InspectorPanel::DrawComponentUI(CameraComponent &comp) {
        ImGui::PushID("Camera");

        static bool headerOpened = true;
        ImGui::SetNextItemOpen(headerOpened, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("View")) {
            auto& camera = comp.m_Camera;

            float fov = glm::degrees(camera.GetPerspectiveFOV()); // convert to degrees for UI
            float near = camera.GetPerspectiveNear();
            float far  = camera.GetPerspectiveFar();

            ImGui::DragFloat("Fov",  &fov,  0.1f, 1.0f, 179.0f); // reasonable FOV range
            ImGui::DragFloat("Near", &near, 0.5f, 0.00001f, 0.8f);
            ImGui::DragFloat("Far",  &far,  1.5f, 0.00001f, 10000.0f);

            // convert to radians again (for fov)
            camera.SetProjectionMatrix(glm::radians(fov), near, far);
        }

        ImGui::PopID();
    }

    void InspectorPanel::DrawComponentUI(EditorState& editorState) {
        ImGui::PushID("General Inspector##");

        auto& entity = editorState.selectedEntity;

        if (!entity) {
            ImGui::PopID();
            return;
        }

        ImGui::PushStyleColor(ImGuiCol_Button,       ImVec4(0.2f, 0.7f, 0.2f, 1.0f)); // Normal state
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.3f, 0.8f, 0.3f, 1.0f)); // Hover state
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.6f, 0.1f, 1.0f)); // Active state
        if (ImGui::Button("Add Component", ImVec2(InspectorWidth - 20, 30))) {
            ImGui::OpenPopup("add_component_popup");
        }
        ImGui::PopStyleColor(3);

        // Component addition popup
        if (ImGui::BeginPopup("add_component_popup")) {
            // second parameter true = "Add Component", false = "Remove Component"
            ManageComponentsMenu(entity, true);
            ImGui::EndPopup();
        }

        ImGui::PushStyleColor(ImGuiCol_Button,       ImVec4(0.7f, 0.2f, 0.2f, 1.0f)); // Normal state
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.8f, 0.3f, 0.3f, 1.0f)); // Hover state
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f)); // Active state
        if (ImGui::Button("Remove Component", ImVec2(InspectorWidth - 20, 30))) {
            ImGui::OpenPopup("remove_component_popup");
        }
        ImGui::PopStyleColor(3);

        // Component deletion popup
        if (ImGui::BeginPopup("remove_component_popup")) {
            // second parameter true = "Add Component", false = "Remove Component"
            ManageComponentsMenu(entity, false);
            ImGui::EndPopup();
        }

        static bool headerOpened = true;
        ImGui::SetNextItemOpen(headerOpened, ImGuiCond_Once);
        if (ImGui::CollapsingHeader("OpenGL State")) {

            bool wireframeMode = opengl::RenderState::GetWireframeMode();
            bool depthTestMode = opengl::RenderState::GetDepthTestMode();
            bool cullingMode   = opengl::RenderState::GetFaceCullingMode();

            ImGui::PushStyleColor(ImGuiCol_Header,
                 wireframeMode ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f)
                               : ImVec4(0.8f, 0.3f, 0.3f, 1.0f)
            );

            if (ImGui::Selectable("Polygon(Wireframe) Mode", wireframeMode, 0, ImVec2(InspectorWidth - 10, 20))) {
                opengl::RenderState::SetWireframeMode(!wireframeMode);
            }

            ImGui::PushStyleColor(ImGuiCol_Header,
                 depthTestMode ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f)
                               : ImVec4(0.8f, 0.3f, 0.3f, 1.0f)
            );

            if (ImGui::Selectable("Depth Test Mode", depthTestMode, 0, ImVec2(InspectorWidth - 10, 20))) {
                opengl::RenderState::SetDepthTestMode(!depthTestMode);
            }

            ImGui::PushStyleColor(ImGuiCol_Header,
                 cullingMode ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f)
                             : ImVec4(0.8f, 0.3f, 0.3f, 1.0f)
            );

            if (ImGui::Selectable("Face Culling Mode", cullingMode, 0, ImVec2(InspectorWidth - 10, 20))) {
                opengl::RenderState::SetFaceCullingMode(!cullingMode);
            }

            ImGui::PopStyleColor(3);
        }

        ImGui::PopID();
    }

    bool InspectorPanel::HasComponentOfType(Entity entity, ComponentType type) {
        switch (type) {
            case ComponentType::Transform: return entity.HasComponent<TransformComponent>();
            case ComponentType::ID:        return entity.HasComponent<IDComponent>();
            case ComponentType::Camera:    return entity.HasComponent<CameraComponent>();
            case ComponentType::Light:     return entity.HasComponent<LightComponent>();
            case ComponentType::Material:  return entity.HasComponent<MaterialComponent>();
            case ComponentType::Mesh:      return entity.HasComponent<MeshComponent>();
            case ComponentType::Model:     return entity.HasComponent<ModelComponent>();
            case ComponentType::Tag:       return entity.HasComponent<TagComponent>();
            default: return false;
        }
    }

    void InspectorPanel::AddComponentToEntity(Entity entity, ComponentType type) {
        switch (type) {
            case ComponentType::Transform: entity.AddComponent<TransformComponent>(); break;
            case ComponentType::Camera:    entity.AddComponent<CameraComponent>();    break;
            case ComponentType::Light:     entity.AddComponent<LightComponent>();     break;
            case ComponentType::Material:  entity.AddComponent<MaterialComponent>();  break;
            case ComponentType::Mesh:      entity.AddComponent<MeshComponent>();      break;
            case ComponentType::Model:     entity.AddComponent<ModelComponent>();     break;
            case ComponentType::Tag:       entity.AddComponent<TagComponent>();       break;
            default: break;
        }
    }

    void InspectorPanel::RemoveComponentFromEntity(Entity entity, ComponentType type) {
        switch (type) {
            case ComponentType::Transform: entity.RemoveComponent<TransformComponent>(); break;
            case ComponentType::Camera:    entity.RemoveComponent<CameraComponent>();    break;
            case ComponentType::Light:     entity.RemoveComponent<LightComponent>();     break;
            case ComponentType::Material:  entity.RemoveComponent<MaterialComponent>();  break;
            case ComponentType::Mesh:      entity.RemoveComponent<MeshComponent>();      break;
            case ComponentType::Model:     entity.RemoveComponent<ModelComponent>();     break;
            case ComponentType::Tag:       entity.RemoveComponent<TagComponent>();       break;
            default: break;
        }
    }

    void InspectorPanel::ManageComponentsMenu(Entity entity, bool AddOrDel) {
        constexpr std::array componentTypes = {
            std::make_pair(ComponentType::Transform, "Transform"),
            std::make_pair(ComponentType::Camera,    "Camera"),
            std::make_pair(ComponentType::Light,     "Light"),
            std::make_pair(ComponentType::Material,  "Material"),
            std::make_pair(ComponentType::Mesh,      "Mesh"),
            std::make_pair(ComponentType::Model,     "Model"),
            std::make_pair(ComponentType::Tag,       "Tag")
        };

        for (const auto& [type, name] : componentTypes) {
            // If addOrDel = true "Add Component", else "Remove Component"
            // Skip if entity already has this component type
            if (AddOrDel) { // Add Component AddOrDel = true
                if (HasComponentOfType(entity, type)) {
                    ImGui::BeginDisabled();
                    ImGui::MenuItem(name, nullptr, false, false);
                    ImGui::EndDisabled();
                } else {
                    if (ImGui::MenuItem(name)) {
                        AddComponentToEntity(entity, type);
                    }
                }
            }
            else { // Remove Component AddOrDel = false
                if (!HasComponentOfType(entity, type)) {
                    ImGui::BeginDisabled();
                    ImGui::MenuItem(name, nullptr, false, false);
                    ImGui::EndDisabled();
                } else {
                    if (ImGui::MenuItem(name)) {
                        RemoveComponentFromEntity(entity, type);
                    }
                }
            }
        }
    }
}
