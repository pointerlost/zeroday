//
// Created by pointerlost on 8/13/25.
//
#pragma once
#include "Scene/Scene.h"
#include "EditorPanel.h"


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
        void drawComponentUI(NameComponent& comp);
        void drawComponentUI(TransformComponent& comp);
        void drawComponentUI(MaterialComponent& comp);
        void drawComponentUI(LightComponent& comp);
        void drawComponentUI(CameraComponent& comp);

        bool deleteEntity(NameComponent& comp, EditorState& state);
    };
}
