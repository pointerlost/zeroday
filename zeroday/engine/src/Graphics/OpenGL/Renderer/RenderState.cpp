//
// Created by pointerlost on 8/29/25.
//
#include "Graphics/OpenGL/Renderer/RenderState.h"
#include "glad/glad.h"

namespace Zeroday::opengl
{
    RenderState::State RenderState::s_State{};

    void RenderState::SetWireframeMode(bool enabled) {
        s_State.m_WireframeMode = enabled;
        glPolygonMode(GL_FRONT_AND_BACK, s_State.m_WireframeMode ? GL_FILL : GL_LINE);
    }

    void RenderState::SetDepthTestMode(bool enabled) {
        s_State.m_DepthTestMode = enabled;
        if (enabled) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
    }

    void RenderState::SetFaceCullingMode(bool enabled) {
        s_State.m_FaceCullingMode = enabled;
        if (enabled) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
    }

    void RenderState::ApplyAllStates() {
        // Background color
        glClearColor(0.04f, 0.05f, 0.06f, 1.0f);
        // Clean the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SetWireframeMode(s_State.m_WireframeMode);
        SetDepthTestMode(s_State.m_DepthTestMode);
        SetFaceCullingMode(s_State.m_FaceCullingMode);
    }
}
