//
// Created by pointerlost on 8/14/25.
//
#pragma once
#include "Editor/Panel/EditorPanel.h"

namespace Graphics { class SceneObjectFactory; }
namespace Graphics { class RenderContext;      }

namespace EDITOR::UI {

    class MenuBarPanel final : public EditorPanel {
    public:
        explicit MenuBarPanel(Graphics::SceneObjectFactory* objectFactory) : sceneObjectFactory(objectFactory) {}
        void draw(EditorState& state) override;

    private:
        Graphics::SceneObjectFactory* sceneObjectFactory;
    };
}
