//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include <memory>
#include <vector>
#include "Panel/EditorPanel.h"

namespace ecs          { class World;         }
namespace EDITOR       { struct EditorState;  }

namespace EDITOR {

    class Editor {
    public:
        explicit Editor(EditorState* s);

        template<typename T>
        void addPanel(std::unique_ptr<T> panel) {
            panels.push_back(std::move(panel));
        }

        void drawUI();

    private:
        std::vector<std::unique_ptr<UI::EditorPanel>> panels;

        EditorState* state;
    };
}
