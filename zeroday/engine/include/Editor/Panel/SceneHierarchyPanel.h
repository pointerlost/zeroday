//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "Editor/Panel/EditorPanel.h"

namespace EDITOR::UI {

    class SceneHierarchyPanel final : public EditorPanel {
    public:
        void draw(EditorState& state) override;
    };
}
