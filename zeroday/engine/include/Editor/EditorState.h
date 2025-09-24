//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "entt/entt.hpp"

namespace Zeroday {
    class Scene;
}

namespace Zeroday::Editor {

    struct EditorState {
        bool isPlayMode    = false; // false = editor mode
        bool showHierarchy = true;
        bool showInspector = true;
        bool showSceneView = true;

        entt::entity selectedEntity = {};
        entt::entity cameraEntity   = {};
        Scene* world = nullptr;

        bool requestShutdown = false;
    };

}
