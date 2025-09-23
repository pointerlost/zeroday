//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include <iostream>


namespace Editor {
    struct EditorState;
}

namespace Editor::UI {

    class EditorPanel {
    public:
        virtual void draw(EditorState& state) = 0;
        virtual ~EditorPanel() = default;
    };
}
