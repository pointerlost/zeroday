//
// Created by pointerlost on 9/1/25.
//
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "core/AssetManager.h"
#include "Editor/State/EditorState.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace {
    // Static service pointers
    Zeroday::MaterialLibrary* g_MaterialLibrary = nullptr;
    Zeroday::MeshLibrary*     g_MeshLibrary     = nullptr;
    ASSET::AssetManager*       g_AssetManager    = nullptr;
    Zeroday::ModelLoader*     g_ModelLoader     = nullptr;
    opengl::RenderContext*     g_RenderContext   = nullptr;
    Zeroday::TextureManager*  g_TextureManager  = nullptr;
    EDITOR::EditorState*       g_EditorState     = nullptr;
}

namespace Services {

    Zeroday::MaterialLibrary *GetMaterialLibrary() { return g_MaterialLibrary; }
    Zeroday::MeshLibrary     *GetMeshLibrary()     { return g_MeshLibrary;     }
    ASSET::AssetManager       *GetAssetManager()    { return g_AssetManager;    }
    Zeroday::ModelLoader     *GetModelLoader()     { return g_ModelLoader;     }
    opengl::RenderContext     *GetRenderContext()   { return g_RenderContext;   }
    Zeroday::TextureManager  *GetTextureManager()  { return g_TextureManager;  }
    EDITOR::EditorState       *GetEditorState()     { return g_EditorState;     }

    void RegisterMaterialLibrary(Zeroday::MaterialLibrary* lib) { g_MaterialLibrary = lib;    }
    void RegisterMeshLibrary(Zeroday::MeshLibrary   *lib)       { g_MeshLibrary     = lib;    }
    void RegisterAssetManager(ASSET::AssetManager    *mngr)      { g_AssetManager    = mngr;   }
    void RegisterModelLoader(Zeroday::ModelLoader   *loader)    { g_ModelLoader     = loader; }
    void RegisterRenderContext(opengl::RenderContext *rc)        { g_RenderContext   = rc;     }
    void RegisterTextureManager(Zeroday::TextureManager *mngr)  { g_TextureManager  = mngr;   }
    void RegisterEditorState(EDITOR::EditorState* state)         { g_EditorState     = state;  }
}