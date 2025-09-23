//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include <memory>
#include <vector>

namespace Zeroday::Editor { struct EditorState;  }

namespace Zeroday::Editor {

    class Editor {
    public:
        explicit Editor(EditorState* s);

        template<typename T>
        void addPanel(Scope<T> panel) {
            panels.push_back(std::move(panel));
        }

        void drawUI();

    private:
        std::vector<Scope<Zeroday::UI::EditorPanel>> panels;

        EditorState* state;
    };
}
