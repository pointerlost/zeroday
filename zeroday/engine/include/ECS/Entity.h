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

    struct GPUEntityMetadata {
        uint32_t VAO;
        uint32_t materialIndex;
        uint32_t transformIndex;
        uint32_t indexCount;
        uint32_t indexOffset;
        uint32_t mobility;
        uint32_t dirtyFlags;
        uint32_t padding;
    };
}
