//
// Created by pointerlost on 8/6/25.
//
#pragma once
#include <iostream>

namespace ECS {
    using Entity = uint32_t;

    // need update! we have to merge entity and this structure
    // to decide which type we will use for object
    enum struct EntityMobility {
        Static,
        Dynamic
    };

    struct EntityInfo {
        bool isVisible = false; // for frustum culling
        EntityMobility mobility = EntityMobility::Dynamic;
    };

}
