//
// Created by pointerlost on 8/14/25.
//
#include "Editor/MenuBarPanel.h"
#include <core/Config.h>
#include <core/EngineConfig.h>
#include <nlohmann/detail/string_concat.hpp>
#include "Core/Engine.h"
#include "Core/Services.h"
#include "Scene/SceneObjectFactory.h"

namespace Zeroday::Editor::UI {

    MenuBarPanel::MenuBarPanel(SceneObjectFactory *objectFactory) : sceneObjectFactory(objectFactory) {

    }

    void MenuBarPanel::Draw(EditorState &state) {

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Scene")) {
                if (ImGui::BeginMenu("Create Empty")) {
                    if (ImGui::BeginMenu("2D")) {
                        if (ImGui::MenuItem("Triangle")) {
                            (void)sceneObjectFactory->CreatePrimitiveObject("triangle");
                        }
                        if (ImGui::MenuItem("Square")) {
                            (void)sceneObjectFactory->CreatePrimitiveObject("square");
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("3D")) {
                        if (ImGui::MenuItem("Cube")) {
                            (void)sceneObjectFactory->CreatePrimitiveObject("cube");
                        }
                        if (ImGui::MenuItem("Sphere")) {
                            (void)sceneObjectFactory->CreatePrimitiveObject("sphere");
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Create Light")) {
                        if (ImGui::MenuItem("Point Light")) {
                            (void)sceneObjectFactory->CreateLight(opengl::LightType::Point);
                        }
                        if (ImGui::MenuItem("Spot Light")) {
                            (void)sceneObjectFactory->CreateLight(opengl::LightType::Spot);
                        }
                        if (ImGui::MenuItem("Directional Light")) {
                            (void)sceneObjectFactory->CreateLight(opengl::LightType::Directional);
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Create Camera")) {
                        if (ImGui::MenuItem("Perspective (3D)")) {
                            (void)sceneObjectFactory->CreateCamera(CameraMode::Perspective);
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Create Model")) {
                        if (ImGui::MenuItem("Cybertruck")) {
                            // (void)sceneObjectFactory->CreateModel(nlohmann::detail::concat(MODEL_DIR, "cybertruck/"));
                        }
                        if (ImGui::MenuItem("house")) {
                            // (void)sceneObjectFactory->CreateModel(nlohmann::detail::concat(MODEL_DIR, "house/source/housetest/houselow.FBX"));
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Performance")) {
                if (ImGui::MenuItem("Profiler")) {
                    m_ShowPerformanceWindow = !m_ShowPerformanceWindow;
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Exit")) {
                Services::GetEngineState()->RequestShutdown();
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        ShowPlayPauseScreen();
        ShowProgressBar();
        if (m_ShowPerformanceWindow) ShowPerformanceProfile();
    }

    void MenuBarPanel::ShowPlayPauseScreen() {
        ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH / 2 - 50, 25), ImGuiCond_Always);
        ImGui::Begin("PlayAndPauseButtons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Play")) {
            PlayStateChanged();
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            PauseStateChanged();
        }

        ImGui::End();
    }

    void MenuBarPanel::ShowProgressBar() {
        if (!showProgressBar) return;

        progressAccumulatedTime += Services::GetEngineState()->GetEditorDeltaTime(); // Sum with delta
        float progress = progressAccumulatedTime / 3.0f;  // X seconds total (in this case 3)

        if (progress >= 1.0f) {
            showProgressBar = false;
            progressAccumulatedTime = 0.0f;
            progress = 1.0f;
            Services::GetEngineState()->SetPaused(false);
        }

        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

        ImGui::Begin("ProgressBar", nullptr,
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoResize   |
            ImGuiWindowFlags_NoMove     |
            ImGuiWindowFlags_NoBackground
        );

        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();

        ImGui::SetWindowFontScale(1.5f);
        ImGui::SetCursorPos(ImVec2(center.x - 92, center.y - 90));
        ImGui::TextColored(m_ScreenTextColor, (progressBarText + "...").c_str());

        ImGui::SetCursorPos(ImVec2(center.x - 175, center.y - 50));
        ImGui::ProgressBar(progress, ImVec2(250, 40));

        ImGui::End();
    }

    void MenuBarPanel::PlayStateChanged() {
        auto* engineState = Services::GetEngineState();
        auto* editorState = Services::GetEditorState();

        // Toggle play state
        engineState->SetIsPlaying(!engineState->IsPlaying());

        // Show appropriate progress bar
        showProgressBar = true;
        progressAccumulatedTime = 0.0f;

        if (engineState->IsPlaying()) {
            progressBarText = "Loading Game";
        } else {
            progressBarText = "Returning to Editor";
        }
        // State changing, editor->game, game->editor
        editorState->ShowInspector = !editorState->ShowInspector;
        editorState->ShowHierarchy = !editorState->ShowHierarchy;
    }

    void MenuBarPanel::PauseStateChanged() {
        auto* engineState = Services::GetEngineState();

        // Toggle pause state
        engineState->SetPaused(!engineState->IsPaused());

        // Show progress bar
        showProgressBar = true;
        progressAccumulatedTime = 0.0f;
        progressBarText = engineState->IsPaused() ? "Pausing" : "Resuming";
    }

    void MenuBarPanel::ShowPerformanceProfile() {
        ImGui::SetNextWindowPos(ImVec2(400,25), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 150));

        ImGui::Begin("Performance Profile", &m_ShowPerformanceWindow, ImGuiWindowFlags_NoResize);

        std::string fpsStr = std::format("FPS: {:.2f}", Services::GetEngineState()->GetEditorFPS());
        ImGui::Text("%s", fpsStr.c_str());

        ImGui::Spacing();

        // std::string frameCount = std::format("FPS: {:.2f}", Services::GetEngineState()->);
        // ImGui::Text("%s", fpsStr.c_str());

        ImGui::End();
    }
}
