//
// Created by pointerlost on 9/15/25.
//
#pragma once
namespace ecs { class World; }

namespace ecs::systems {

    class ISystem {
    public:
        virtual ~ISystem() = default;
        virtual void Update(World* world) = 0;
    };

    template <typename Derived>
    class System : public ISystem {
    public:
        void Update(World* world) override {
            static_cast<Derived*>(this)->Update(world);
        }
    };

}
