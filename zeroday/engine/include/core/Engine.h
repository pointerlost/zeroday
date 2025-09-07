#pragma once
#include <memory>
#include "core/Window.h"
#include "ImGui/ImGuiLayer.h"
#include "ECS/World.h"
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

namespace core
{

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

		std::unique_ptr<EDITOR::EditorState> editorState;
		std::unique_ptr<EDITOR::Editor> editor;
		std::unique_ptr<Window> window;
		std::unique_ptr<ECS::World> world;
		std::unique_ptr<Graphics::MeshLibrary> meshLibrary;
		std::unique_ptr<ASSET::AssetManager> assetManager;
		std::unique_ptr<Graphics::SceneObjectFactory> sceneObjectFactory;
		std::unique_ptr<OpenGL::Renderer> renderer;
		std::unique_ptr<Graphics::ModelLoader> modelLoader;
		std::unique_ptr<ENGINE::UI::ImGuiLayer> imGuiLayer;
		std::unique_ptr<Graphics::MaterialLibrary> materialLibrary;
		std::unique_ptr<Graphics::TextureManager> textureManager;
		std::unique_ptr<OpenGL::BufferManager> glBufferManager;
		std::unique_ptr<OpenGL::RenderContext> renderContext;

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
		void glfwRenderEventStuff() const  noexcept;

		void UpdatePhase();
		void UIPhase();
		void RenderPhase();
		void CleanupPhase();

		bool m_RequestShutdown = false;
	};
};

