//
// Created by pointerlost on 10/1/25.
//
#pragma once


namespace Zeroday {

    class Timer {
    public:
        void Start();
        void Stop();
        void Update();
        void Reset();

        [[nodiscard]] float GetElapsedTime() const { return m_ElapsedTime; }
        [[nodiscard]] float GetDeltaTime() const   { return m_DeltaTime;   }
        [[nodiscard]] float GetCurrentTime() const { return m_CurrentFrameTime; }
        [[nodiscard]] bool IsRunning() const { return m_Running;  }

        void CalculateFPS();
        [[nodiscard]] float GetFPS() const { return m_CurrentFPS; }

        Timer()  = default;
        ~Timer() = default;
        Timer(const Timer&) = default;
        Timer& operator=(const Timer&) = default;

    private:
        float m_ElapsedTime      = 0.0f;  // Total time
        float m_LastFrameTime    = 0.0f;
        float m_CurrentFrameTime = 0.0f;
        float m_DeltaTime        = 0.0f;  // Last - Curr
        bool m_Running           = false; // Is timer active

        // FPS
        int m_FrameCount = 0;
        float m_FPSTimeAccum = 0.0f;
        float m_CurrentFPS = 0.0f;
    };
}