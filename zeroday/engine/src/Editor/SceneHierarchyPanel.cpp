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
        ImGui::SetNextWindowPos({0, MainMenuBarHeight}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({HierarchyWidth, HierarchyHeight}, ImGuiCond_Always);

        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize);

        auto view = state.scene->GetAllEntitiesWith<NameComponent>();
        for (auto [enttEntity, nameComp] : view.each()) {

            // Create entity as Entity (we have Entity so for editor we will use type-casting)
            const Entity entity(enttEntity, state.scene);

            if (ImGui::Button(nameComp.name.c_str())) {
                state.selectedEntity = entity;
            } else if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered()) {
                state.selectedEntity = Entity{};
            }
        }

        ImGui::End();
    }
}
