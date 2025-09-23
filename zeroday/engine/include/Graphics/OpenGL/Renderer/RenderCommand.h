//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include "Scene/Entity.h"

namespace Zeroday::opengl {

    // unique for per-object
    struct RenderCommandMDI {
        uint vao             = 0;
        uint materialIndex   = 0;
        uint transformIndex  = 0;
        uint indexCount      = 0;
        uint indexOffset     = 0;
    };
    static_assert(std::is_trivially_copyable_v<RenderCommandMDI>, "it's not!");

    // OpenGL 4.6 specification, the order is matter!
    struct DrawElementsIndirectCommand {
        uint count         = 0; // number of indices
        uint instanceCount = 0; // usually 1 for now
        uint firstIndex    = 0; // index offset (in *indices*, not bytes)
        uint baseVertex    = 0; // 0 if not using glDrawElementsBaseVertex
        uint baseInstance  = 0; // index into our payload array (SSBO)
    };

    // Per-draw payload your shader will index with gl_BaseInstance
    struct DrawPayloadGPU {
        int transformIndex = -1;
        int materialIndex  = -1;
        int meshId         = -1; // for debugging or LOD table
        int EntityId       = -1;
    };

}
