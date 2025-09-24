//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "EditorPanel.h"

namespace Zeroday::Editor::UI {

    class SceneHierarchyPanel final : public EditorPanel {
    public:
        void Draw(EditorState& state) override;
    };
}
