//
// Created by pointerlost on 9/18/25.
//
#pragma once
#include "entt/entt.hpp"

namespace Zeroday {

    class Scene {
    public:
        Scene();
        ~Scene() = default;



    private:
        entt::registry m_Registry;
    };
}