#pragma once
#include <memory>
#include "Base.h"
#include "core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Editor/Editor.h"
#include "../Editor/EditorState.h"
#include "core/AssetManager.h"
#include "Graphics/OpenGL/Renderer/Renderer3D.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Graphics/OpenGL/Renderer/GPURenderer.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "Scene/SceneObjectFactory.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace Zeroday {

	class Engine {
	public:
		Engine() = default;
		~Engine();

		[[nodiscard]] bool Run();
		[[nodiscard]] bool InitResources();
		void InitServices();
		void InitPointerObjects();

		void EditorStateLoop(GLFWwindow* glfwWin);
		void GameStateLoop();

	private:
		Scope<EditoR::EditorState> m_EditorState;
		Scope<EditoR::Editor> m_Editor;
		Scope<Window> m_Window;
		Scope<Scene> m_Scene;
		Scope<opengl::Renderer3D> m_Renderer3D;
		Scope<SceneObjectFactory> m_SceneObjectFactory;
		Scope<UI::ImGuiLayer> m_ImGuiLayer;
		Scope<MeshLibrary> g_MeshLibrary;
		Scope<AssetManager> g_AssetManager;
		Scope<ModelLoader> g_ModelLoader;
		Scope<MaterialLibrary> g_MaterialLibrary;
		Scope<TextureManager> g_TextureManager;
		Scope<::opengl::RenderContext> g_RenderContext;

		void InitWindow();
		void InitCallBack();
		void InitAssetManager();
		void InitWorld();
		void InitOpenGLBufferState();
		void InitModel();
		void InitTexture();
		void InitMaterial();
		void InitMesh();
		void InitRender();
		void InitScene();
		void InitEditor();
		void InitImGui();

		void OpenGLSetUpResources() noexcept;
		void OpenGLRenderStuff()    noexcept;
		void glfwRenderEventStuff() const noexcept;

		void UpdatePhase();
		void UIPhase();
		void RenderPhase();
		void CleanupPhase();

		bool m_RequestShutdown = false;
	};
};

