//
// Created by pointerlost on 8/14/25.
//
#pragma once
#include <imgui.h>
#include <string>
#include "EditorPanel.h"

namespace Zeroday { class SceneObjectFactory; }

namespace Zeroday::Editor::UI {

    class MenuBarPanel final : public EditorPanel {
    public:
        explicit MenuBarPanel(SceneObjectFactory* objectFactory);
        void Draw(EditorState& state) override;

    private:
        SceneObjectFactory* sceneObjectFactory;

        void ShowPlayPauseScreen();
        void ShowProgressBar();

        void PlayStateChanged();
        void PauseStateChanged();

        void ShowPerformanceProfile();
        bool m_ShowPerformanceWindow = false;

        bool showProgressBar = false;
        float progressAccumulatedTime = 0.0;
        std::string progressBarText = {};
        float progressStartTime = 0.0;

        ImVec4 m_ScreenTextColor = ImVec4(0.78, 0.75, 0.76, 1.0);
    };
}
