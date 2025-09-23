//
// Created by pointerlost on 8/14/25.
//
#pragma once
#include "EditorPanel.h"

namespace Zeroday { class SceneObjectFactory; }
namespace Zeroday { class RenderContext;      }

namespace Editor::UI {

    class MenuBarPanel final : public EditorPanel {
    public:
        explicit MenuBarPanel(Zeroday::SceneObjectFactory* objectFactory) : sceneObjectFactory(objectFactory) {}
        void draw(EditorState& state) override;

    private:
        Zeroday::SceneObjectFactory* sceneObjectFactory;
    };
}
