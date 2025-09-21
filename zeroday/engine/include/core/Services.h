//
// Created by pointerlost on 9/1/25.
//
#pragma once

namespace Zeroday { class MaterialLibrary; class MeshLibrary; class ModelLoader; class TextureManager; }
namespace ASSET    { class AssetManager;  }
namespace opengl   { class RenderContext; }
namespace EDITOR   { class EditorState;   }

namespace Services {

    Zeroday::MaterialLibrary *GetMaterialLibrary();
    Zeroday::MeshLibrary     *GetMeshLibrary();
    ASSET::AssetManager       *GetAssetManager();
    Zeroday::ModelLoader     *GetModelLoader();
    opengl::RenderContext     *GetRenderContext();
    Zeroday::TextureManager  *GetTextureManager();
    EDITOR::EditorState       *GetEditorState();

    // Service registration
    void RegisterMaterialLibrary(Zeroday::MaterialLibrary *lib);
    void RegisterMeshLibrary(Zeroday::MeshLibrary *lib);
    void RegisterAssetManager(ASSET::AssetManager  *mngr);
    void RegisterModelLoader(Zeroday::ModelLoader *loader);
    void RegisterRenderContext(opengl::RenderContext *rc);
    void RegisterTextureManager(Zeroday::TextureManager *mngr);
    void RegisterEditorState(EDITOR::EditorState *state);
}