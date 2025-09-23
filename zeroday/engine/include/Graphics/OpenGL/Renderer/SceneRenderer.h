//
// Created by pointerlost on 9/23/25.
//
#pragma once
#include <vector>

namespace Zeroday {
    class Scene;

    namespace opengl {
        struct RenderCommandMDI;
        struct MaterialSSBO;
        struct LightSSBO;
        struct TransformSSBO;
        struct CameraUBO;
    }
}


namespace Zeroday::opengl {

    struct ExtractResult {
        std::vector<TransformSSBO> transforms;
        std::vector<MaterialSSBO> materials;
        std::vector<LightSSBO> lights;
        std::vector<RenderCommandMDI> renderCommands;
        std::vector<CameraUBO> cameras;

        int mainCameraIndex = 0;
    };

    class SceneRenderer {
    public:
        static ExtractResult ExtractRenderables(Scene* scene);

    private:
        // Helpers for extractions
        static void ExtractLights(Scene* scene, ExtractResult& result);
        static void ExtractCamera(Scene* scene, ExtractResult& result);
    };
}
