//
// Created by pointerlost on 10/1/25.
//
#include "Core/Timer.h"
#include <GLFW/glfw3.h>

namespace Zeroday {

    void Timer::Start() {
        m_Running = true;
        m_LastFrameTime = static_cast<float>(glfwGetTime());
        m_ElapsedTime = 0.0f;
    }

    void Timer::Stop() {
        m_Running = false;
    }

    void Timer::Update() {
        if (!m_Running) return;

        m_CurrentFrameTime = static_cast<float>(glfwGetTime());
        m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime;
        m_ElapsedTime += m_DeltaTime;
        m_LastFrameTime = m_CurrentFrameTime;

        CalculateFPS();
    }

    void Timer::Reset() {
        m_ElapsedTime = 0.0f;
        m_DeltaTime = 0.0f;
    }

    void Timer::CalculateFPS() {
        m_FrameCount++;
        m_FPSTimeAccum += m_DeltaTime;

        if (m_FPSTimeAccum >= 1.0f) {
            m_CurrentFPS = static_cast<float>(m_FrameCount) / m_FPSTimeAccum;
            m_FrameCount = 0;
            m_FPSTimeAccum = 0.0f;
        }
    }
}
