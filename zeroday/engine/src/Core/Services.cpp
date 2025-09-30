//
// Created by pointerlost on 9/1/25.
//
#include "Core/Services.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Core/AssetManager.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace {
    // Static service pointers
    Zeroday::MeshLibrary* g_MeshLibrary         = nullptr;
    Zeroday::AssetManager* g_AssetManager       = nullptr;
    Zeroday::Graphics::ModelLoader* g_ModelLoader   = nullptr;
    Zeroday::opengl::RenderContext* g_RenderContext = nullptr;
    Zeroday::Editor::EditorState* g_EditorState     = nullptr;
    float DeltaTime = 0.0f;
    float lastFrameTime = 0.0f;
}

namespace Zeroday::Services {
    MeshLibrary *GetMeshLibrary()         { return g_MeshLibrary;     }
    AssetManager *GetAssetManager()       { return g_AssetManager;    }
    Graphics::ModelLoader *GetModelLoader()   { return g_ModelLoader;     }
    opengl::RenderContext *GetRenderContext() { return g_RenderContext;   }
    Editor::EditorState *GetEditorState()     { return g_EditorState;     }
    float GetTime() { return DeltaTime; }
    void SetTime() {
        const double currentTime = glfwGetTime();
        DeltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;
    }

    void RegisterMeshLibrary(MeshLibrary   *lib)          { g_MeshLibrary     = lib;    }
    void RegisterAssetManager(AssetManager    *mngr)      { g_AssetManager    = mngr;   }
    void RegisterModelLoader(Graphics::ModelLoader   *loader) { g_ModelLoader = loader; }
    void RegisterRenderContext(opengl::RenderContext *rc) { g_RenderContext   = rc;     }
    void RegisterEditorState(Editor::EditorState* state)  { g_EditorState     = state;  }
}