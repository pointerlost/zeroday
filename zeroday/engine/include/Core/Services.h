//
// Created by pointerlost on 9/1/25.
//
#pragma once

namespace Zeroday {
    namespace Graphics {
        class ModelLoader;
    }

    namespace Editor {
        struct EditorState;
    }

    class AssetManager;
    class MeshLibrary;

    namespace opengl {
        class RenderContext;
    }

    namespace EditoR {
        class EditorState;
    }
}

namespace Zeroday::Services {

    MeshLibrary *GetMeshLibrary();
    AssetManager *GetAssetManager();
    Graphics::ModelLoader *GetModelLoader();
    opengl::RenderContext *GetRenderContext();
    Editor::EditorState *GetEditorState();

    // Service registration
    void RegisterMeshLibrary(MeshLibrary *lib);
    void RegisterAssetManager(AssetManager  *mngr);
    void RegisterModelLoader(Graphics::ModelLoader *loader);
    void RegisterRenderContext(opengl::RenderContext *rc);
    void RegisterEditorState(Editor::EditorState *state);
}