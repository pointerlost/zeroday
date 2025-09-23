//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "Scene/Scene.h"
#include "EditorPanel.h"


namespace ecs {
    struct NameComponent;
    struct TransformComponent;
    struct LightComponent;
    struct MeshComponent;
    struct MaterialComponent;
    struct CameraComponent;
}
namespace Zeroday::Editor { struct EditorState; }

namespace Zeroday::Editor::UI {

    class InspectorPanel final : public EditorPanel {
    public:
        void draw(EditorState& state) override;

    private:
        // Component based UI design
        void drawComponentUI(ecs::NameComponent& comp);
        void drawComponentUI(ecs::TransformComponent& comp);
        void drawComponentUI(ecs::MaterialComponent& comp);
        void drawComponentUI(ecs::LightComponent& comp);
        void drawComponentUI(ecs::CameraComponent& comp);

        bool deleteEntity(ecs::NameComponent& comp, EditorState& state);
    };
}
