//
// Created by pointerlost on 8/14/25.
//
#pragma once
#include <string>

#include "EditorPanel.h"

namespace Zeroday { class SceneObjectFactory; }

namespace Zeroday::Editor::UI {

    class MenuBarPanel final : public EditorPanel {
    public:
        explicit MenuBarPanel(SceneObjectFactory* objectFactory) : sceneObjectFactory(objectFactory) {}
        void Draw(EditorState& state) override;

    private:
        SceneObjectFactory* sceneObjectFactory;

        void ShowPlayPauseScreen();
        void ShowTextWithProgressBar();

        bool showProgressBar = false;
        std::string progressBarText = {};
        float progressStartTime = 0.0;
    };
}
