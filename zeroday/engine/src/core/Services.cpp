//
// Created by pointerlost on 9/1/25.
//
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "core/AssetManager.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace {
    // Static service pointers
    Zeroday::MaterialLibrary* g_MaterialLibrary = nullptr;
    Zeroday::MeshLibrary* g_MeshLibrary         = nullptr;
    Zeroday::AssetManager* g_AssetManager       = nullptr;
    Zeroday::Graphics::ModelLoader* g_ModelLoader   = nullptr;
    Zeroday::opengl::RenderContext* g_RenderContext = nullptr;
    Zeroday::TextureManager* g_TextureManager       = nullptr;
    Zeroday::Editor::EditorState* g_EditorState     = nullptr;
}

namespace Zeroday::Services {

    MaterialLibrary *GetMaterialLibrary() { return g_MaterialLibrary; }
    MeshLibrary *GetMeshLibrary()         { return g_MeshLibrary;     }
    AssetManager *GetAssetManager()       { return g_AssetManager;    }
    Graphics::ModelLoader *GetModelLoader()   { return g_ModelLoader;     }
    opengl::RenderContext *GetRenderContext() { return g_RenderContext;   }
    TextureManager  *GetTextureManager()      { return g_TextureManager;  }
    Editor::EditorState *GetEditorState()     { return g_EditorState;     }

    void RegisterMaterialLibrary(MaterialLibrary* lib)    { g_MaterialLibrary = lib;    }
    void RegisterMeshLibrary(MeshLibrary   *lib)          { g_MeshLibrary     = lib;    }
    void RegisterAssetManager(AssetManager    *mngr)      { g_AssetManager    = mngr;   }
    void RegisterModelLoader(Graphics::ModelLoader   *loader) { g_ModelLoader = loader; }
    void RegisterRenderContext(opengl::RenderContext *rc) { g_RenderContext   = rc;     }
    void RegisterTextureManager(TextureManager *mngr)     { g_TextureManager  = mngr;   }
    void RegisterEditorState(Editor::EditorState* state)  { g_EditorState     = state;  }
}