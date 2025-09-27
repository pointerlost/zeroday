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
    struct NameComponent;
}

namespace Zeroday::Editor::UI {

    class InspectorPanel final : public EditorPanel {
    public:
        void Draw(EditorState& state) override;

    private:
        // Component based UI design
        void DrawComponentUI(NameComponent& comp);
        void DrawComponentUI(TransformComponent& comp);
        void DrawComponentUI(MaterialComponent& comp);
        void DrawComponentUI(LightComponent& comp);
        void DrawComponentUI(CameraComponent& comp);
    };
}
