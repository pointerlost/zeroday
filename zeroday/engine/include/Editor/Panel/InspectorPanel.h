//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "../../ECS/World.h"
#include "Editor/Panel/EditorPanel.h"


namespace ECS {
    struct NameComponent;
    struct TransformComponent;
    struct LightComponent;
    struct MeshComponent;
    struct MaterialComponent;
    struct CameraComponent;
}
namespace EDITOR { struct EditorState; }

namespace EDITOR::UI {

    class InspectorPanel final : public EditorPanel {
    public:
        void draw(EditorState& state) override;

    private:
        // Component based UI design
        void drawComponentUI(ECS::NameComponent& comp);
        void drawComponentUI(ECS::TransformComponent& comp);
        void drawComponentUI(ECS::MaterialComponent& comp);
        void drawComponentUI(ECS::LightComponent& comp);
        void drawComponentUI(ECS::CameraComponent& comp);

        bool deleteEntity(ECS::NameComponent& comp, EditorState& state);
    };
}
