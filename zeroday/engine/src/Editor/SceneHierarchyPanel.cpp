//
// Created by pointerlost on 8/13/25.
//
#include "Editor/SceneHierarchyPanel.h"
#include <imgui.h>
#include "core/EngineConfig.h"
#include "Editor/EditorState.h"
#include "Scene/Components.h"
#include "Scene/Scene.h"

namespace Zeroday::Editor::UI {

    void SceneHierarchyPanel::Draw(EditorState &state) {
        static bool deleteRequested = false;
        static ImVec2 deletePopupSize = ImVec2(340, 160); // fixed size

        ImGui::SetNextWindowPos({0, MainMenuBarHeight}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({HierarchyWidth, HierarchyHeight}, ImGuiCond_Always);
        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize);

        // Can be more optimize need fix!
        auto view = state.scene->GetAllEntitiesWith<TagComponent>();
        for (auto [enttEntity, nameComp] : view.each()) {
            Entity entity(enttEntity, state.scene);

            bool isSelected = (state.selectedEntity == entity);
            ImGui::Selectable(nameComp.tag.c_str(), isSelected);

            // Left click
            if (ImGui::IsItemClicked(0))
                state.selectedEntity = entity;

            // Right click
            if (ImGui::IsItemClicked(1)) {
                state.selectedEntity = entity;
                ImGui::OpenPopup(("EntityMenu##" + std::to_string(entity.GetUUID())).c_str());
            }

            // Button
            if (ImGui::BeginPopup(("EntityMenu##" + std::to_string(entity.GetUUID())).c_str())) {
                // Left click
                if (ImGui::MenuItem("Delete")) {
                    deleteRequested = true;
                }
                ImGui::EndPopup();
            }
        }

        // Release
        if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered())
            state.selectedEntity = Entity{};

        ImGui::End();

        // ---------------- Delete Confirmation Popup ----------------
        if (deleteRequested && state.selectedEntity) {
            ImGui::OpenPopup("DeletePopup");
            deleteRequested = false;
        }

        // Fixed size and centered position before BeginPopupModal
        ImGui::SetNextWindowSize(deletePopupSize, ImGuiCond_Always);
        ImGui::SetNextWindowPos(
            ImVec2(
                ImGui::GetIO().DisplaySize.x * 0.5f - deletePopupSize.x * 0.5f,
                ImGui::GetIO().DisplaySize.y * 0.5f - deletePopupSize.y * 0.5f
            ),
            ImGuiCond_Always
        );

        if (ImGui::BeginPopupModal("DeletePopup", nullptr, ImGuiWindowFlags_NoResize)) {
            ImGui::TextWrapped("Are you sure you want to delete this entity?");
            ImGui::Dummy(ImVec2(0, 20)); // spacing before buttons

            // centered yes | no buttons
            constexpr float buttonWidth = 120.0;
            constexpr float buttonHeight = 50.0;
            constexpr float totalWidth = buttonWidth * 2 + 10; // 10 px spacing between buttons
            const float startX = (deletePopupSize.x - totalWidth) * 0.5f;

            ImGui::SetCursorPosX(startX);
            if (ImGui::Button("Yes", ImVec2(buttonWidth, buttonHeight))) {
                state.scene->DestroyEntity(state.selectedEntity);
                state.selectedEntity = Entity{};
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            ImGui::SetCursorPosX(startX + buttonWidth + 10);
            if (ImGui::Button("No", ImVec2(buttonWidth, buttonHeight))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
}
