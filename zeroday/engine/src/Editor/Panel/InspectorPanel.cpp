//
// Created by pointerlost on 8/13/25.
//
#include "Editor/Panel/InspectorPanel.h"
#include "Editor/State/EditorState.h"
#include "core/EngineConfig.h"
#include <imgui.h>
#include "glm/ext.hpp"
#include "Scene/World.h"
#include "Graphics/OpenGL/Lighting/Light.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Transformations/Transformations.h"

namespace EDITOR::UI {

    void InspectorPanel::draw(EditorState &state) {
        const auto world = state.world;

        float inspectorX = InspectorWidth;
        float inspectorY = InspectorHeight;
        ImGui::SetNextWindowPos({SCR_WIDTH - InspectorWidth, MainMenuBarHeight}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({inspectorX, inspectorY}, ImGuiCond_Always);

        ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoResize);

        auto* nameComp         = world->GetComponent<ecs::NameComponent>(state.selectedEntity);
        auto* transformComp= world->GetComponent<ecs::TransformComponent>(state.selectedEntity);
        auto* matComp        = world->GetComponent<ecs::MaterialComponent>(state.selectedEntity);
        auto* lightComp         = world->GetComponent<ecs::LightComponent>(state.selectedEntity);
        auto* camComp        = world->GetComponent<ecs::CameraComponent>(state.selectedEntity);

        if (nameComp && deleteEntity(*nameComp, state)) {
            ImGui::End();
            return;
        }
        if (nameComp)      drawComponentUI(*nameComp);
        if (transformComp) drawComponentUI(*transformComp);
        if (camComp)       drawComponentUI(*camComp);
        if (lightComp)     drawComponentUI(*lightComp);
        if (matComp)       drawComponentUI(*matComp);

        ImGui::End();
    }

    void InspectorPanel::drawComponentUI(ecs::NameComponent &comp) {
        char buffer[128];
        strncpy( buffer, comp.name.c_str(), sizeof(buffer) );

        if (ImGui::CollapsingHeader("Name")) {
            ImGui::SetNextItemWidth(400);
            ImGui::InputText("##NameInput ", buffer, sizeof(buffer));
            comp.name = buffer;
        }
    }

    void InspectorPanel::drawComponentUI(ecs::TransformComponent &comp) {
        ImGui::PushID("Transform");
        const auto& transform = comp.transform;

        glm::vec3& pos = transform->getPosition();
        glm::vec3& rot = transform->getRotation();
        glm::vec3& scl = transform->getScale();

        if (ImGui::CollapsingHeader("Transform")) {
            if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1)) transform->setPosition(pos);
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(rot), 0.1)) transform->setRotation(rot);
            if (ImGui::DragFloat3("Scale",    glm::value_ptr(scl), 0.1, 0.05)) transform->setScale(scl);
        }

        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(ecs::MaterialComponent &comp) {
        ImGui::PushID("Material");
        auto& material = comp.instance;

        if (ImGui::CollapsingHeader("Material")) {
            auto baseColor = material->getBaseColor();
            if (ImGui::ColorEdit4("Base Color R",  &baseColor.x,  ImGuiColorEditFlags_None)) {
                material->setBaseColor(baseColor);
            }
        }

        float metallic = material->getMetallic();
        if (ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f)) {
            material->setMetallic(metallic);
        }

        float roughness = material->getRoughness();
        if (ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f)) {
            material->setRoughness(roughness);
        }

        glm::vec3 emissive = material->getEmissive();
        if (ImGui::ColorEdit3("Emissive", &emissive.x)) {
            material->setEmissive(emissive);
        }

        if (ImGui::CollapsingHeader("Texture")) {
            if (ImGui::Button("On")) {

            }
            if (ImGui::Button("Off")) {

            }
        }
        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(ecs::LightComponent &comp) {
        ImGui::PushID("Light");
        const auto& light = comp.light;

        // Light Type selection
        const char* lightTypes[] = { "Directional", "Point", "Spot" };
        int currentType = static_cast<int>(light->getType());
        if (ImGui::Combo("Type", &currentType, lightTypes, IM_ARRAYSIZE(lightTypes))) {
            // Handle type change if needed - you might want to create a new Light object
            // with the appropriate type and copy relevant properties
        }

        ImGui::Separator();

        // Radiance (color) with color picker
        glm::vec3 radiance = light->getRadiance();
        float radianceArray[3] = { radiance.x, radiance.y, radiance.z };
        if (ImGui::ColorEdit3("Radiance", radianceArray)) {
            light->setRadiance(glm::vec3(radianceArray[0], radianceArray[1], radianceArray[2]));
        }

        // Intensity
        float intensity = light->getIntensity();
        if (ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f, 100.0f)) {
            light->setIntensity(intensity);
        }

        ImGui::Separator();

        // Position (for point and spot lights)
        if (light->getType() != LIGHTING::LightType::Directional) {
            glm::vec3 position = light->getPosition();
            float posArray[3] = { position.x, position.y, position.z };
            if (ImGui::DragFloat3("Position", posArray, 0.1f)) {
                light->setPosition(glm::vec3(posArray[0], posArray[1], posArray[2]));
            }
        }

        // Direction (for directional and spot lights)
        if (light->getType() != LIGHTING::LightType::Point) {
            glm::vec3 direction = light->getDirection();
            float dirArray[3] = { direction.x, direction.y, direction.z };
            if (ImGui::DragFloat3("Direction", dirArray, 0.01f, -1.0f, 1.0f)) {
                glm::vec3 newDir = glm::vec3(dirArray[0], dirArray[1], dirArray[2]);
                if (glm::length(newDir) > 0.001f) {
                    light->setDirection(glm::normalize(newDir));
                }
            }
        }

        ImGui::Separator();

        // Spotlight-specific parameters
        if (light->getType() == LIGHTING::LightType::Spot) {
            // Convert cosine angles to degrees for more intuitive editing
            float cutOffDeg      = glm::degrees(glm::acos(light->getCutOff()));
            float outerCutOffDeg = glm::degrees(glm::acos(light->getOuterCutOff()));

            if (ImGui::DragFloat("Cut Off (degrees)", &cutOffDeg, 1.0f, 1.0f, 89.0f)) {
                light->setCutOff(static_cast<float>(glm::cos(glm::radians(cutOffDeg))));
            }

            if (ImGui::DragFloat("Outer Cut Off (degrees)", &outerCutOffDeg, 1.0f, 1.0f, 89.0f)) {
                light->setOuterCutOff(static_cast<float>(glm::cos(glm::radians(outerCutOffDeg))));
            }

            // Ensure outer cut off is always larger than inner cut off
            if (outerCutOffDeg <= cutOffDeg) {
                outerCutOffDeg = cutOffDeg + 1.0f;
                light->setOuterCutOff(static_cast<float>(glm::cos(glm::radians(outerCutOffDeg))));
            }
        }

        // Attenuation parameters (for point and spot lights)
        if (light->getType() != LIGHTING::LightType::Directional) {
            ImGui::Separator();
            ImGui::Text("Attenuation");

            float constant = light->getConstant();
            if (ImGui::DragFloat("Constant", &constant, 0.01f, 0.0f, 10.0f)) {
                light->setConstant(constant);
            }

            float linear = light->getLinear();
            if (ImGui::DragFloat("Linear", &linear, 0.001f, 0.0f, 1.0f)) {
                light->setLinear(linear);
            }

            float quadratic = light->getQuadratic();
            if (ImGui::DragFloat("Quadratic", &quadratic, 0.0001f, 0.0f, 0.01f, "%.6f")) {
                light->setQuadratic(quadratic);
            }
        }

        ImGui::PopID();
    }

    void InspectorPanel::drawComponentUI(ecs::CameraComponent &comp) {
        ImGui::PushID("Camera");

        if (ImGui::CollapsingHeader("View")) {
            auto& camera = comp;

            float fov  = comp.getFov();
            float near = comp.getNearPlane();
            float far  = comp.getFarPlane();

            ImGui::DragFloat("Fov",  &fov,  0.1f, 0.0f, 8000.0f);
            ImGui::DragFloat("Near", &near, 0.5f, 0.0f, 0.8f);
            ImGui::DragFloat("Far",  &far,  1.5f, 0.0f, 10000.0f);

            comp.setProjectionMatrix(fov, near, far);
        }

        ImGui::PopID();
    }

    bool InspectorPanel::deleteEntity(ecs::NameComponent &comp, EditorState &state) {
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
                    deleteWindowOpen = false;
                    if (state.cameraEntity != state.selectedEntity) {
                        deleted = true;
                        state.world->RemoveEntity(state.selectedEntity);
                        state.selectedEntity = -1;
                    }
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
