//
// Created by pointerlost on 9/19/25.
//
#pragma once
#include <typeindex>
#include "Components.h"
#include "core/Assert.h"
#include "Scene.h"

namespace Zeroday {

    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, Scene* scene);
        Entity(const Entity& other) = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            ZD_ASSERT(!HasComponent<T>(), "Entity already has component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
            m_Scene->OnComponentAdded<T>(*this, component);
            return component;
        }

        template<typename T>
        [[nodiscard]] bool HasComponent() const {
            return m_Scene->m_Registry.any_of<T>(m_Handle);
        }

        template <typename T>
        [[nodiscard]] T& GetComponent() {
            ZD_ASSERT(HasComponent<T>(), "Entity does not have component!");
            return m_Scene->m_Registry.get<T>(m_Handle);
        }

        template<typename T>
        [[nodiscard]] T* TryGetComponent() {
            return m_Scene->m_Registry.try_get<T>(m_Handle);
        }

        template<typename... Components>
        auto TryGetAllComponents() {
            return std::make_tuple(TryGetComponent<Components>()...);
        }

        template<typename T>
        void RemoveComponent() {
            ZD_ASSERT(HasComponent<T>(), "Entity does not have component!");
            m_Scene->m_Registry.remove<T>(m_Handle);
        }

        [[nodiscard]] bool IsValid() const {
            return m_Handle != entt::null && m_Scene != nullptr;
        }

        [[nodiscard]] operator bool() const { return m_Handle != entt::null; }
        operator entt::entity() const { return m_Handle; }
        operator uint32_t() const { return static_cast<uint32_t>(m_Handle); }

        UUID GetUUID() { return GetComponent<IDComponent>().ID; }
        const std::string& GetName() { return GetComponent<NameComponent>().name; }

        [[nodiscard]] bool operator==(const Entity& other) const {
            return m_Handle == other.m_Handle && m_Scene == other.m_Scene;
        }

        [[nodiscard]] bool operator!=(const Entity& other) const {
            return !(*this == other);
        }

    private:
        entt::entity m_Handle{ entt::null };
        Scene* m_Scene{ nullptr };
    };
}
