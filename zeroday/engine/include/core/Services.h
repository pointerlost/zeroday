//
// Created by pointerlost on 9/1/25.
//
#pragma once

namespace Zeroday {
    namespace Editor {
        struct EditorState;
    }

    class AssetManager;
    class MaterialLibrary;
    class MeshLibrary;
    class ModelLoader;
    class TextureManager;

    namespace opengl {
        class RenderContext;
    }

    namespace EditoR {
        class EditorState;
    }
}

namespace Zeroday::Services {

    MaterialLibrary *GetMaterialLibrary();
    MeshLibrary *GetMeshLibrary();
    AssetManager *GetAssetManager();
    ModelLoader *GetModelLoader();
    opengl::RenderContext *GetRenderContext();
    TextureManager *GetTextureManager();
    Editor::EditorState *GetEditorState();

    // Service registration
    void RegisterMaterialLibrary(MaterialLibrary *lib);
    void RegisterMeshLibrary(MeshLibrary *lib);
    void RegisterAssetManager(AssetManager  *mngr);
    void RegisterModelLoader(ModelLoader *loader);
    void RegisterRenderContext(opengl::RenderContext *rc);
    void RegisterTextureManager(TextureManager *mngr);
    void RegisterEditorState(Editor::EditorState *state);
}