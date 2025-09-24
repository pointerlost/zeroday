//
// Created by pointerlost on 8/13/25.
//
#pragma once

namespace Zeroday::Editor {
    struct EditorState;
}

namespace Zeroday::Editor::UI {

    class EditorPanel {
    public:
        virtual void Draw(EditorState& state) = 0;
        virtual ~EditorPanel() = default;
    };
}
