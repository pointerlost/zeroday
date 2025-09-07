//
// Created by pointerlost on 9/1/25.
//
#pragma once

namespace Graphics { class MaterialLibrary; class MeshLibrary; class ModelLoader; class TextureManager; }
namespace ASSET    { class AssetManager;  }
namespace OpenGL   { class RenderContext; }
namespace EDITOR   { class EditorState;   }

namespace Services {

    Graphics::MaterialLibrary *GetMaterialLibrary();
    Graphics::MeshLibrary     *GetMeshLibrary();
    ASSET::AssetManager       *GetAssetManager();
    Graphics::ModelLoader     *GetModelLoader();
    OpenGL::RenderContext     *GetRenderContext();
    Graphics::TextureManager  *GetTextureManager();
    EDITOR::EditorState       *GetEditorState();

    // Service registration
    void RegisterMaterialLibrary(Graphics::MaterialLibrary *lib);
    void RegisterMeshLibrary(Graphics::MeshLibrary *lib);
    void RegisterAssetManager(ASSET::AssetManager  *mngr);
    void RegisterModelLoader(Graphics::ModelLoader *loader);
    void RegisterRenderContext(OpenGL::RenderContext *rc);
    void RegisterTextureManager(Graphics::TextureManager *mngr);
    void RegisterEditorState(EDITOR::EditorState *state);
}