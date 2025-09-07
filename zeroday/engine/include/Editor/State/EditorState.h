//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "ECS/Entity.h"

// forward declarations
namespace ECS { class World; }

namespace EDITOR {

    struct EditorState {
        bool isPlayMode = false;    // false = editor mode
        bool showHierarchy = true;
        bool showInspector = true;
        bool showSceneView = true;

        ECS::Entity selectedEntity = -1;
        ECS::Entity cameraEntity = -1;
        ECS::World* world = nullptr;

        bool requestShutdown = false;
    };

}
