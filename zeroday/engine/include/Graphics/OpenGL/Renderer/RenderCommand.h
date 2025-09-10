//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include <vector>
#include <glad/glad.h>
#include "ECS/Entity.h"
#include "Graphics/OpenGL/Macros.h"

namespace OpenGL {

    // unique for per-object
    struct RenderCommandMDI {
        ECS::Entity entity   = INVALID_ENTITY;
        GLuint VAO           = 0;
        uint32_t indexCount  = UINT32_MAX;
        uint32_t indexOffset = UINT32_MAX;
        int materialIndex    = -1;
        int transformIndex   = -1;
        int subMeshIndex     = -1;
        // storing visibility and mobility etc. for frustum culling or batches
        ECS::EntityInfo entityInfo{};
    };

    // OpenGL 4.6 specification, don't change the order!
    struct DrawElementsIndirectCommand {
        GLuint count         = 0; // number of indices
        GLuint instanceCount = 0; // usually 1 for now
        GLuint firstIndex    = 0; // index offset (in *indices*, not bytes)
        GLuint baseVertex    = 0; // 0 if not using glDrawElementsBaseVertex
        GLuint baseInstance  = 0; // index into our payload array (SSBO)
    };

    // Per-draw payload your shader will index with gl_BaseInstance
    struct DrawPayloadGPU {
        int transformIndex = -1;
        int materialIndex  = -1;
        int meshID         = -1; // for debugging or LOD table
        int pad            = -1; // keep std430 alignment happy
    };

}
