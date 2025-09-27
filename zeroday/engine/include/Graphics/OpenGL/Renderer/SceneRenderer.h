//
// Created by pointerlost on 9/23/25.
//
#pragma once
#include <vector>
#include "Graphics/OpenGL/GPU_buffers.h"

namespace Zeroday {
    class Scene;

    namespace opengl {
        struct RenderCommandMDI;
    }
}


namespace Zeroday::opengl {

    struct ExtractResult {
        std::vector<TransformSSBO> transforms;
        std::vector<MaterialSSBO> materials;
        std::vector<LightSSBO> lights;
        std::vector<RenderCommandMDI> renderCommands;
        CameraUBO camera;
        GlobalUBO globalData;
    };

    class SceneRenderer {
    public:
        static ExtractResult ExtractRenderables(Scene* scene);

    private:
        // Helpers for extractions
        static void ExtractLights(Scene* scene, ExtractResult& result);
        static void ExtractCamera(Scene* scene, ExtractResult& result);
        static void ExtractGlobalData(Scene* scene, ExtractResult& result);

        static inline glm::vec3 g_GlobalAmbient = glm::vec3(0.1f);
    };

}
