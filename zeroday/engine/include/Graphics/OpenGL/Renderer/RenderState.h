//
// Created by pointerlost on 8/22/25.
//
#pragma once
#include <vector>
#include "Graphics/OpenGL/BufferManager.h"
#include "Graphics/OpenGL/Renderer/RenderCommand.h"
#include "Graphics/OpenGL/Macros.h"

namespace OpenGL {

    struct RenderState {
    public:
        Graphics::GlobalFragmentSSBO* globalFragmentSSBO;

        std::vector<RenderCommandMDI> mdiStaticBuild;  // Build on scene changes
        std::vector<RenderCommandMDI> mdiDynamicBuild; // rebuilt per frame

        std::vector<MDIBatch> staticBatches;  // GPU-ready batches
        std::vector<MDIBatch> dynamicBatches; // GPU-ready batches
    };

    struct EditorRenderState final : public RenderState {
        BufferInfo staticBuffer;
        BufferInfo dynamicBuffer;
    };
    struct GameRenderState   final : public RenderState {
        BufferInfo staticBuffer;
        BufferInfo dynamicBuffer;
    };

}
