//
// Created by pointerlost on 9/21/25.
//
#include "Scene/Entity.h"

namespace Zeroday {

    Entity::Entity(entt::entity handle, Scene *scene)
        : m_Handle(handle), m_Scene(scene)
    {
    }
}
