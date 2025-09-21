//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include "Graphics/OpenGL/BufferManager.h"

namespace opengl {

    struct RenderState {
    public:
        // std::vector<RenderCommandMDI> mdiStaticBuild;  // Build on scene changes
        // std::vector<RenderCommandMDI> mdiDynamicBuild; // rebuilt per frame
        // std::vector<MDIBatch> staticBatches;  // GPU-ready batches
        // std::vector<MDIBatch> dynamicBatches; // GPU-ready batches
    };

    struct EditorRenderState final : public RenderState {
    };

    struct GameRenderState   final : public RenderState {
    };

}
