//
// Created by pointerlost on 9/15/25.
//
#pragma once
namespace ecs { class Scene; }

namespace ecs::systems {

    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void Update(Scene* world) = 0;
    };

    template <typename Derived>
    class System : public ISystem {
    public:
        void Update(Scene* world) override {
            static_cast<Derived*>(this)->Update(world);
        }
    };

}
