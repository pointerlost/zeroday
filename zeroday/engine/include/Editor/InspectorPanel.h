//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "EditorPanel.h"
#include "Scene/Entity.h"

namespace Zeroday {
    struct CameraComponent;
    struct LightComponent;
    struct MaterialComponent;
    struct TransformComponent;
    struct TagComponent;
}

namespace Zeroday::Editor::UI {

    class InspectorPanel final : public EditorPanel {
    public:
        void Draw(EditorState& state) override;

    private:
        // Component based UI design
        void DrawComponentUI(TagComponent& comp);
        void DrawComponentUI(TransformComponent& comp);
        void DrawComponentUI(MaterialComponent& comp);
        void DrawComponentUI(LightComponent& comp);
        void DrawComponentUI(CameraComponent& comp);
        void DrawComponentUI(EditorState& state);

        // helpers
        bool HasComponentOfType(Entity entity, ComponentType type);
        void AddComponentToEntity(Entity entity, ComponentType type);
        void RemoveComponentFromEntity(Entity entity, ComponentType type);
        void ManageComponentsMenu(Entity entity, bool AddOrDel);
    };
}
