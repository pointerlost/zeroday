//
// Created by pointerlost on 8/13/25.
//
#include "../../include/Editor/SceneHierarchyPanel.h"
#include <imgui.h>
#include "core/EngineConfig.h"
#include "../../include/Editor/EditorState.h"

namespace Zeroday::Ecs { struct NameComponent; }

namespace Editor::UI {

    void SceneHierarchyPanel::draw(EditorState &state) {
        ImGui::SetNextWindowPos({0, MainMenuBarHeight}, ImGuiCond_Always);
        ImGui::SetNextWindowSize({HierarchyWidth, HierarchyHeight}, ImGuiCond_Always);

        ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize);

        for (auto& [entity, nameComp] : state.world->getStorage<Zeroday::Ecs::NameComponent>().GetAll()) {
            if (ImGui::Button(nameComp.name.c_str())) {
                state.selectedEntity = entity;
            } else if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && !ImGui::IsWindowHovered()) {
                state.selectedEntity = -1;
            }
        }

        ImGui::End();
    }
}
