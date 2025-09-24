//
// Created by pointerlost on 9/16/25.
//
#include <Scene/transform_system.h>
#include "core/Services.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace ecs::systems {

    void TransformSystem::Update(Scene* world) {
        auto* context = Zeroday::Services::GetRenderContext();
        auto& ers = context->GetEditorRenderState();
    }
}
