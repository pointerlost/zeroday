//
// Created by pointerlost on 8/22/25.
//
#pragma once

namespace Zeroday::opengl {

    class RenderState {
    private:
        struct State {
            bool m_WireframeMode   = true;
            bool m_DepthTestMode   = true;
            bool m_FaceCullingMode = false;
        };

        static State s_State;

    public:
        static void SetWireframeMode(bool enabled);
        static void SetDepthTestMode(bool enabled);
        static void SetFaceCullingMode(bool enabled);

        static bool GetWireframeMode()   { return s_State.m_WireframeMode;   }
        static bool GetDepthTestMode()   { return s_State.m_DepthTestMode;   }
        static bool GetFaceCullingMode() { return s_State.m_FaceCullingMode; }

        static bool IsWireFrameMode()   { return s_State.m_WireframeMode;   }
        static bool IsDepthTestMode()   { return s_State.m_DepthTestMode;   }
        static bool IsFaceCullingMode() { return s_State.m_FaceCullingMode; }

        static void ApplyAllStates();
    };

}
