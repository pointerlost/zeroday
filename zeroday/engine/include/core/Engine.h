#pragma once
#include <memory>
#include "core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "Editor/Editor.h"
#include "Editor/State/EditorState.h"
#include "core/AssetManager.h"
#include "Graphics/OpenGL/Renderer/Renderer.h"
#include "Graphics/OpenGL/Mesh/MeshLibrary.h"
#include "Graphics/OpenGL/Material/material_lib.h"
#include "Graphics/OpenGL/Model/ModelLoader.h"
#include "Graphics/OpenGL/BufferManager.h"
#include "Graphics/OpenGL/Textures/TextureManager.h"
#include "Scene/SceneObjectFactory.h"
#include "Graphics/OpenGL/Renderer/RenderContext.h"

namespace Zeroday {

	class Engine {
	public:
		Engine() = default;
		~Engine();

		[[nodiscard]] bool run();
		[[nodiscard]] bool initResources();
		void InitServices();
		void InitPointerObjects();

		void EditorStateLoop(GLFWwindow* glfwWin);
		void GameStateLoop();

	private:

		std::unique_ptr<EDITOR::EditorState> m_EditorState;
		std::unique_ptr<EDITOR::Editor> m_Editor;
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<opengl::BufferManager> m_GLBufferManager;
		std::unique_ptr<ecs::World> m_World;
		std::unique_ptr<opengl::Renderer> m_Renderer;
		std::unique_ptr<Zeroday::SceneObjectFactory> m_SceneObjectFactory;
		std::unique_ptr<ENGINE::UI::ImGuiLayer> m_ImGuiLayer;
		std::unique_ptr<Zeroday::MeshLibrary> g_MeshLibrary;
		std::unique_ptr<ASSET::AssetManager> g_AssetManager;
		std::unique_ptr<Zeroday::ModelLoader> g_ModelLoader;
		std::unique_ptr<Zeroday::MaterialLibrary> g_MaterialLibrary;
		std::unique_ptr<Zeroday::TextureManager> g_TextureManager;
		std::unique_ptr<opengl::RenderContext> g_RenderContext;

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

