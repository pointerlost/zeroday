//
// Created by pointerlost on 8/14/25.
//
#include "../../include/Editor/MenuBarPanel.h"
#include <imgui.h>
#include <core/Config.h>
#include <nlohmann/detail/string_concat.hpp>
#include "../../include/Editor/EditorState.h"
#include "Scene/SceneObjectFactory.h"

namespace Editor::UI {

    void MenuBarPanel::draw(EditorState &state) {

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("SCENE")) {
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
                            (void)sceneObjectFactory->CreateLight(LIGHTING::LightType::Point);
                        }
                        if (ImGui::MenuItem("Spot Light")) {
                            (void)sceneObjectFactory->CreateLight(LIGHTING::LightType::Spot);
                        }
                        if (ImGui::MenuItem("Directional Light")) {
                            (void)sceneObjectFactory->CreateLight(LIGHTING::LightType::Directional);
                        }
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Create Camera")) {
                        if (ImGui::MenuItem("Perspective (3D)")) {
                            (void)sceneObjectFactory->CreateCamera(ecs::CameraMode::Primary, ecs::PERSPECTIVE);
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
        }

        if (ImGui::MenuItem("Exit")) {
            state.requestShutdown = true;
        }

        ImGui::EndMainMenuBar();
    }
}
