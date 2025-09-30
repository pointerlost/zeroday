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
        bool IsPlayMode    = false; // false = editor mode
        bool ShowHierarchy = true;
        bool ShowInspector = true;

        Entity selectedEntity = {};
        Entity cameraEntity   = {};
        Scene* scene = nullptr;

        bool RequestShutdown = false;
    };

}
