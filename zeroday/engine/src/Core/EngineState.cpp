//
// Created by pointerlost on 10/1/25.
//
#include "Core/EngineState.h"

namespace Zeroday {

    void EngineState::UpdateTimers() {
        m_EditorTimer.Update();
        if (m_IsPlaying && !m_IsPaused) {
            m_GameTimer.Update();
        }
    }

    void EngineState::SetIsPlaying(bool playing) {
        m_IsPlaying = playing;
        if (playing) {
            // Reset game timer when entering play mode
            m_GameTimer.Reset();
            m_GameTimer.Start();
        }
    }

    void EngineState::StartTimers() {
        m_EditorTimer.Start();
        m_GameTimer.Start();
    }

}
