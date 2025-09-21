//
// Created by pointerlost on 8/13/25.
//
#include "Editor/Panel/SceneHierarchyPanel.h"
#include <imgui.h>
#include "core/EngineConfig.h"
#include "Editor/State/EditorState.h"

namespace Zeroday::Ecs { struct NameComponent; }

namespace EDITOR::UI {

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
