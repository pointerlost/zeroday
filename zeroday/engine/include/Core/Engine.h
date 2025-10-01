#pragma once
#include "Base.h"
#include "EngineState.h"
#include "Timer.h"
#include "Core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Editor/Editor.h"
#include "Editor/EditorState.h"
#include "Core/AssetManager.h"
#include "Graphics/OpenGL/Renderer/Renderer3D.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Scene/SceneObjectFactory.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace Zeroday {

	class Engine {
	public:
		Engine();
		~Engine();

		[[nodiscard]] bool Run();
		[[nodiscard]] bool InitResources();
		void InitServices();
		void InitPointerObjects();

		void GameLoop(GLFWwindow* glfwWin);

	private:
		Scope<Editor::EditorState> m_EditorState;
		Scope<Editor::Editor> m_Editor;
		Scope<Window> m_Window;
		Scope<Scene> m_Scene;
		Scope<opengl::Renderer3D> m_Renderer3D;
		Scope<SceneObjectFactory> m_SceneObjectFactory;
		Scope<UI::ImGuiLayer> m_ImGuiLayer;
		Scope<MeshLibrary> g_MeshLibrary;
		Scope<AssetManager> g_AssetManager;
		Scope<Graphics::ModelLoader> g_ModelLoader;
		Scope<opengl::RenderContext> g_RenderContext;

		void InitEngineState();
		void InitWindow();
		void InitCallBack();
		void InitAssetManager();
		void InitModel();
		void InitMesh();
		void InitRender();
		void InitScene();
		void InitEditor();
		void InitImGui();

		void OpenGLState();
		void glfwEventState() const;

		void UpdatePhase();
		void UIPhase();
		void RenderPhase();
		void CleanupPhase();
		void RenderGameWindow();
		void EditorToGameTime();

		Scope<EngineState> m_EngineState;
	};
};

