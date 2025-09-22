//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "Scene/Entity.h"

// forward declarations
namespace ecs { class Scene; }

namespace EDITOR {

    struct EditorState {
        bool isPlayMode    = false;    // false = editor mode
        bool showHierarchy = true;
        bool showInspector = true;
        bool showSceneView = true;

        entt::entity selectedEntity = {};
        entt::entity cameraEntity   = {};
        ecs::Scene* world = nullptr;

        bool requestShutdown = false;
    };

}
