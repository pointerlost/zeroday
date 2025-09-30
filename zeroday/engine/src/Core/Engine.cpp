#include "Core/Engine.h"
#include "Core/Config.h"
#include "CallBack/CallBack.h"
#include <imgui_impl_opengl3.h>
#include <Core/EngineConfig.h>
#include <Input/Input.h>
#include "Core/Services.h"
#include "Editor/InspectorPanel.h"
#include "Editor/MenuBarPanel.h"
#include "Editor/SceneHierarchyPanel.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"
#include "Scene/SceneObjectFactory.h"

namespace Zeroday {

	Engine::~Engine() {
		if (m_ImGuiLayer) {
			m_ImGuiLayer->Shutdown();
		}
	}

	bool Engine::Run() {
		const auto glfwWin = m_Window->GetGLFWwindow();

		if (!glfwWin) {
			Error("[Engine::run] GLFW window returning nullptr!");
			return false;
		}
		glfwSwapInterval(0);

		// engine life loop
		EditorStateLoop(glfwWin);

		return true;
	}

	bool Engine::InitResources() {
		// Load materials
		if (!g_AssetManager->LoadMaterialsFromFolder(std::string(MATERIAL_JSON_PATH) + "materials.json")) {
			Error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		Info("[Engine::initResources] Materials loaded successfully!");

		// Load Shaders
		m_Renderer3D->InitEditorState();

		Info("[Engine::initResources] Shaders loaded successfully!");

		m_ImGuiLayer->Init(m_Window->GetGLFWwindow());

		(void)m_SceneObjectFactory->CreateLight(opengl::LightType::Point, "Point Light");
		(void)m_SceneObjectFactory->CreatePrimitiveObject("cube", "Cube");

		Info("Engine initResources successful!");

		return true;
	}

	void Engine::InitServices() {
		Services::RegisterMeshLibrary(g_MeshLibrary.get());
		Services::RegisterAssetManager(g_AssetManager.get());
		Services::RegisterModelLoader(g_ModelLoader.get());
		Services::RegisterRenderContext(g_RenderContext.get());
		Services::RegisterEditorState(m_EditorState.get());
	}

	void Engine::InitPointerObjects() {
		try {
			InitWindow();
			InitCallBack();
			InitAssetManager();
			InitMesh();
			InitModel();
			InitScene();
			InitRender();
			InitEditor();
			InitImGui();
			InitServices();

			Info("[Engine::InitPointerObjects] Successful!");
		}
		catch (const std::exception& e) {
			Error("[Engine::initPointerObjects] FAILED! Error: " + std::string(e.what()));
			throw;
		}
	}

    void Engine::EditorStateLoop(GLFWwindow* glfwWin) {
		while (!glfwWindowShouldClose(glfwWin) && !m_EditorState->requestShutdown) {
			UpdatePhase();
			RenderPhase();
			UIPhase();
			CleanupPhase();
		}
    }

    void Engine::InitWindow() {
        m_Window = CreateScope<Window>();

		if (!m_Window->InitResources()) {
			Error("Engine::initWindow FAILED because of initResources!");
			throw std::runtime_error("Failed to initialize window FAILED because of initResources!");
		}

		Info("[Engine::InitWindow] Window initialized successfully!]");
    }

    void Engine::InitCallBack() {
		CallBack::InitResources(m_Window->GetGLFWwindow());

		Info("[Engine::InitCallBack] initialized successfully!");
	}

    void Engine::InitAssetManager() {
		g_AssetManager = CreateScope<AssetManager>();

		Info("[Engine::InitAssetManager] AssetManager initialized successfully!");
    }

    void Engine::InitMesh() {
		g_MeshLibrary = CreateScope<MeshLibrary>();

		Info("[Engine::InitMesh] MeshLibrary initialized successfully!");
	}

	void Engine::InitModel() {
		g_ModelLoader = CreateScope<Graphics::ModelLoader>();

		Info("[Engine::InitModel] Model Loader initialized successfully!]");
	}

	void Engine::InitScene() {
		m_Scene = CreateScope<Scene>();

		Info("[Engine::InitWorld] World initialized successfully!");

		m_SceneObjectFactory = CreateScope<SceneObjectFactory>(*m_Scene);

		Info("SceneObjectFactory initialized successfully!");
	}

	void Engine::InitRender() {
		g_RenderContext = CreateScope<opengl::RenderContext>();

		Info("[Engine::InitRender] RenderContext initialized successfully!");

		m_Renderer3D = CreateScope<opengl::Renderer3D>(m_Scene.get());

		Info("[Engine::InitRender] RendererManager initialized successfully!");
	}

	void Engine::InitEditor() {
		m_EditorState = CreateScope<Editor::EditorState>();

		Info("[Engine::InitEditor] Editor initialized successfully!");

		Entity editorCamera = m_SceneObjectFactory->CreateCamera(CameraMode::Perspective, "Editor Camera");
		if (!editorCamera) {
			Error("[Engine::InitEditor] Failed to create editor camera!");
			return;
		}
		editorCamera.TryGetComponent<CameraComponent>()->m_Camera.SetViewportSize(SCR_WIDTH, SCR_HEIGHT);

		m_EditorState->cameraEntity = editorCamera;
		m_EditorState->scene = m_Scene.get();

		m_Editor = CreateScope<Editor::Editor>(m_EditorState.get());
		// order is matter don't change it, hierarchy panel managing entity deletions
		// if you change the order so can cause crashes
		m_Editor->addPanel(CreateScope<Editor::UI::InspectorPanel>());
		m_Editor->addPanel(CreateScope<Editor::UI::MenuBarPanel>(m_SceneObjectFactory.get()));
		m_Editor->addPanel(CreateScope<Editor::UI::SceneHierarchyPanel>());

		Info("[Engine::InitEditor] Editor initialized successfully!]");
	}

	void Engine::InitImGui() {
		m_ImGuiLayer = CreateScope<UI::ImGuiLayer>(m_Window->GetGLFWwindow(), m_SceneObjectFactory.get());

		Info("[Engine::InitImGui] ImGuiLayer initialized successfully!");
	}

	void Engine::OpenGLRenderStuff() {
		opengl::RenderState::ApplyAllStates();
	}

	void Engine::glfwRenderEvent() const {
		glfwPollEvents();
		glfwSwapBuffers(m_Window->GetGLFWwindow());
	}

	void Engine::UpdatePhase() {
		Input::Update();
		// world->update();
	}

	void Engine::UIPhase() {
		m_ImGuiLayer->BeginFrame();
		m_Editor->drawUI();
		m_ImGuiLayer->EndFrame();
	}

	void Engine::RenderPhase() {
		OpenGLRenderStuff();
		m_Renderer3D->RenderEditorState();
	}

	void Engine::CleanupPhase() {
		glfwRenderEvent();
		m_Scene->CleanUpResources();
	}

	void Engine::EditorToGameState() {
		auto renderablesObjects = opengl::SceneRenderer::ExtractRenderables(m_Scene.get());
	}

	void Engine::TakeSnapshot() {
	}
}
