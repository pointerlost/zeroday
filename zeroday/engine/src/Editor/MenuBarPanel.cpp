//
// Created by pointerlost on 8/14/25.
//
#include "Editor/MenuBarPanel.h"
#include <imgui.h>
#include <core/Config.h>
#include <core/EngineConfig.h>
#include <nlohmann/detail/string_concat.hpp>
#include "Core/Engine.h"
#include "Core/Services.h"
#include "Scene/SceneObjectFactory.h"

namespace Zeroday::Editor::UI {

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
                            (void)sceneObjectFactory->CreateModel(nlohmann::detail::concat(MODEL_DIR, "/house/source/housetest/houselow.FBX"));
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Exit")) {
                Services::GetEngineState()->RequestShutdown();
            }

            ImGui::EndMainMenuBar();
        }

        ShowPlayPauseScreen();
        ShowProgressBar();
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
        ImGui::TextColored(ImVec4(0.78, 0.75, 0.76, 1.0), (progressBarText + "...").c_str());

        ImGui::SetCursorPos(ImVec2(center.x - 175, center.y - 50));
        ImGui::ProgressBar(progress, ImVec2(250, 40));

        ImGui::End();
    }

    void MenuBarPanel::PlayStateChanged() {
        auto* engineState = Services::GetEngineState();

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
        // editorState->ShowInspector = !editorState->ShowInspector;
        // editorState->ShowHierarchy = !editorState->ShowHierarchy;
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
}
