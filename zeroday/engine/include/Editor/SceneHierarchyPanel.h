//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "EditorPanel.h"

namespace Editor::UI {

    class SceneHierarchyPanel final : public EditorPanel {
    public:
        void draw(EditorState& state) override;
    };
}
