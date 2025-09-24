//
// Created by pointerlost on 8/13/25.
//
#include "Editor/Editor.h"
#include "Editor/EditorPanel.h"
#include "Editor/EditorState.h"

namespace Zeroday::Editor {

    Editor::Editor(EditorState *s) : state(s) {}

    void Editor::drawUI() {
        for (const auto& panel : panels) {
            panel->Draw(*state);
        }
    }
}
