//
// Created by pointerlost on 8/14/25.
//
#include "Editor/MenuBarPanel.h"
#include <imgui.h>
#include <core/Config.h>
#include <core/EngineConfig.h>
#include <nlohmann/detail/string_concat.hpp>

#include "Core/Services.h"
#include "Editor/EditorState.h"
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
                state.RequestShutdown = true;
            }

            ImGui::EndMainMenuBar();
        }

        ShowPlayPauseScreen();
        ShowTextWithProgressBar();
    }

    void MenuBarPanel::ShowPlayPauseScreen() {
        const auto& editorState = Services::GetEditorState();
        ImGui::SetNextWindowPos(ImVec2(SCR_WIDTH / 2 - 50, 25), ImGuiCond_Always);
        ImGui::Begin("PlayAndPauseButtons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("Play")) {
            showProgressBar = true;
            progressBarText = "Loading";
            // State changing, editor->game, game->editor
            editorState->IsPlayMode    = !editorState->IsPlayMode;
            editorState->ShowInspector = !editorState->ShowInspector;
            editorState->ShowHierarchy = !editorState->ShowHierarchy;
        }

        ImGui::SameLine();
        // Sameline with Play button
        if (ImGui::Button("Pause")) {
            showProgressBar = true;
            progressBarText = "Paused";
            editorState->ShowInspector = !editorState->ShowInspector;
            editorState->ShowHierarchy = !editorState->ShowHierarchy;
        }

        ImGui::End();
    }

    void MenuBarPanel::ShowTextWithProgressBar() {
        if (!showProgressBar) return;

        float currTime = Services::GetTime();
        float elapsed = currTime - progressStartTime;
        float progress = elapsed / 2.0f;

        if (progress > 1.0f) {
            showProgressBar = false;
            progress = 1.0f;
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
        ImGui::SetCursorPos(ImVec2(center.x - 60, center.y - 50));
        ImGui::TextColored(ImVec4(0.1, 0.05, 0.6, 1.0), (progressBarText + "...").c_str());

        ImGui::SetCursorPos(ImVec2(center.x - 175, center.y - 50));
        ImGui::ProgressBar(progress, ImVec2(250, 0));

        ImGui::End();
    }
}
