//
// Created by pointerlost on 10/1/25.
//
#pragma once
#include "Timer.h"

namespace Zeroday {

    struct EngineState {
    private:
        bool m_Running = true;
        bool m_RequestShutdown = false;
        bool m_IsPlaying = false;
        bool m_IsPaused = false;

        Timer m_EditorTimer;
        Timer m_GameTimer;

    public:
        EngineState() = default;

        Timer& GetEditorTimer() { return m_EditorTimer; }
        Timer& GetGameTimer()   { return m_GameTimer;   }

        [[nodiscard]] float GetEditorFPS() const { return m_EditorTimer.GetFPS(); }
        [[nodiscard]] float GetGameFPS()   const { return m_GameTimer.GetFPS();   }

        // State control
        void SetRunning(bool running) { m_Running  = running; }
        void SetPaused(bool paused)   { m_IsPaused = paused;  }
        void SetIsPlaying(bool playing);

        void RequestShutdown() { m_RequestShutdown = true; }

        // State query
        [[nodiscard]] bool IsRunning() const { return m_Running;   }
        [[nodiscard]] bool IsPaused()  const { return m_IsPaused;  }
        [[nodiscard]] bool IsPlaying() const { return m_IsPlaying; }
        [[nodiscard]] bool ShutdownRequested() const { return m_RequestShutdown; }

        void StartTimers();
        void UpdateTimers();

        [[nodiscard]] float GetEditorTime() const { return m_EditorTimer.GetElapsedTime(); }
        [[nodiscard]] float GetGameTime()   const { return m_GameTimer.GetElapsedTime();   }
        [[nodiscard]] float GetEditorDeltaTime() const { return m_EditorTimer.GetDeltaTime(); }
        [[nodiscard]] float GetGameDeltaTime()   const { return m_GameTimer.GetDeltaTime();   }
        [[nodiscard]] float GetDeltaTime()   const {
            return m_IsPlaying ? GetGameDeltaTime() : GetEditorDeltaTime();
        }
        [[nodiscard]] float GetCurrentTime() const {
            return m_IsPlaying ? GetGameTime() : GetEditorTime();
        }
    };
}
