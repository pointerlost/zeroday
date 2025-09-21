#include "Graphics/OpenGL/Renderer/Renderer.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"
#include "core/AssetManager.h"
#include "core/Logger.h"
#include "core/Services.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Macros.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "Graphics/OpenGL/Renderer/RenderCommand.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace opengl {

    void Renderer::Render()
    {
        BufferManager::UpdateBuffers(world);

        ASSET::AssetManager::getShader(BASIC_SHADER)->bind();

        BufferManager::HandleBatchReBuilds(world);
        BufferManager::BuildAllBatchesForEditorState(world);
        BufferManager::DrawAllBatchesForEditorState();
    }

}