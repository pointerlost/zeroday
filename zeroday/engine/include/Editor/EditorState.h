//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "Scene/Entity.h"

namespace Zeroday {
    class Scene;
}

namespace Zeroday::Editor {

    struct EditorState {
        bool isPlayMode    = false; // false = editor mode
        bool showHierarchy = true;
        bool showInspector = true;
        bool showSceneView = true;

        Entity selectedEntity = {};
        Entity cameraEntity   = {};
        Scene* scene = nullptr;

        bool requestShutdown = false;
    };

}
