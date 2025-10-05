#include "Core/Engine.h"
#include "Core/Config.h"
#include "CallBack/CallBack.h"
#include <imgui_impl_opengl3.h>
#include <thread>
#include <Input/Input.h>
#include <nlohmann/detail/string_concat.hpp>

#include "Core/Services.h"
#include "Editor/InspectorPanel.h"
#include "Editor/MenuBarPanel.h"
#include "Editor/SceneHierarchyPanel.h"
#include "Graphics/OpenGL/Camera/CameraInput.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Graphics/OpenGL/Renderer/SceneRenderer.h"
#include "Graphics/OpenGL/Textures/Textures.h"
#include "Scene/SceneObjectFactory.h"

namespace Zeroday {

	Engine::Engine() {}

	Engine::~Engine() {
		if (m_ImGuiLayer) {
			m_ImGuiLayer->Shutdown();
		}
	}

	bool Engine::Run() {
		const auto glfwWin = m_Window->GetGLFWwindow();
		glfwSwapInterval(0);

		// engine life loop
		GameLoop(glfwWin);

		return true;
	}

	bool Engine::InitResources() {
		// Start Global Timer
		m_EngineState->StartTimers();

		// Load materials
		if (!g_AssetManager->LoadMaterialsFromFolder(std::string(MATERIAL_JSON_PATH) + "materials.json")) {
			Error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		Info("[Engine::initResources] Materials loaded successfully!");

		// Load shaders
		if (!Services::GetAssetManager()->LoadAllShaders()) {
			Error("[Engine::initResources] LoadAllShaders FAILED!");
		}

		(void)m_SceneObjectFactory->CreateLight(opengl::LightType::Point, "Point Light");
		(void)m_SceneObjectFactory->CreatePrimitiveObject("cube", "Cube");

		(void)m_SceneObjectFactory->TestModelLoader(nlohmann::detail::concat(MODEL_DIR, "Sponza/glTF/Sponza.gltf"));
		(void)m_SceneObjectFactory->TestModelLoader(nlohmann::detail::concat(MODEL_DIR, "island_tree/island_tree_02_4k.gltf"));

		// Load meshes into GPU
		g_MeshLibrary->GetMeshData3D()->SetupMeshes();

		m_Renderer3D->Init();

		Info("[Engine::initResources] Shaders loaded successfully!");

		m_ImGuiLayer->Init(m_Window->GetGLFWwindow());

		Info("Engine initResources successful!");

		return true;
	}

	void Engine::InitServices() {
		Services::RegisterMeshLibrary(g_MeshLibrary.get());
		Services::RegisterAssetManager(g_AssetManager.get());
		Services::RegisterModelLoader(g_ModelLoader.get());
		Services::RegisterRenderContext(g_RenderContext.get());
		Services::RegisterEditorState(m_EditorState.get());
		Services::RegisterEngineState(m_EngineState.get());
	}

	void Engine::InitPointerObjects() {
		try {
			InitEngineState();
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

    void Engine::GameLoop(GLFWwindow* glfwWin) {
		while (!glfwWindowShouldClose(glfwWin) && !m_EngineState->ShutdownRequested()) {
			OpenGLState();

			UpdatePhase();
			RenderPhase();
			UIPhase();
			CleanupPhase();
		}
    }

    void Engine::InitEngineState() {
		m_EngineState = CreateScope<EngineState>();

		Info("[Engine::InitEngineState] Initialization successful!");
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

		Info("[Engine::InitImGui] +ImGuiLayer initialized successfully!");
	}

	void Engine::OpenGLState() {
		opengl::RenderState::ApplyAllStates();
	}

	void Engine::glfwEventState() const {
		glfwPollEvents();
		glfwSwapBuffers(m_Window->GetGLFWwindow());
	}

	void Engine::UpdatePhase() {
		Input::Update();
		CameraInput::Update();
		m_EngineState->UpdateTimers();
		m_Scene->Update();
	}

	void Engine::UIPhase() {
		m_ImGuiLayer->BeginFrame();
		m_Editor->drawUI();
		RenderGameWindow();
		m_ImGuiLayer->EndFrame();
	}

	void Engine::RenderPhase() {
		if (m_EngineState->IsPaused()) return;
		m_Renderer3D->Render();
	}

	void Engine::CleanupPhase() {
		glfwEventState();
		m_Scene->CleanUpResources();
	}

	void Engine::RenderGameWindow() {
		if (m_EngineState->IsPlaying()) {
			auto editorCamera = m_EditorState->cameraEntity.GetComponent<CameraComponent>().m_Camera;
			auto pos = opengl::SceneRenderer::GetViewportSize(m_EngineState->IsPlaying());
			const auto viewPort = ImVec2(pos.first, pos.second);
			editorCamera.SetViewportSize(viewPort.x, viewPort.y);

			const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(viewPort, ImGuiCond_Appearing);
			ImGui::SetNextWindowFocus();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
			ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(80, 40, 40, 255));
			ImGui::Begin("Zeroday Game", nullptr,
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoScrollWithMouse |
				ImGuiWindowFlags_NoCollapse
			);

			const ImVec2 windowPos = ImGui::GetWindowPos();
			const ImVec2 windowSize = ImGui::GetWindowSize();
			const float centeredX = windowPos.x + (windowSize.x - viewPort.x) * 0.5f;
			const float centeredY = windowPos.y + (windowSize.y - viewPort.y) * 0.5f;

			glViewport((int)centeredX, (int)centeredY, (int)viewPort.x, (int)viewPort.y);

			ImGui::End();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
		}
	}

	void Engine::EditorToGameTime() {
		auto* editorState = Services::GetEditorState();

		auto editorCamera = editorState->cameraEntity.GetComponent<CameraComponent>();
		auto [first, second] = opengl::SceneRenderer::GetViewportSize(m_EngineState->IsPlaying());
		editorCamera.m_Camera.SetViewportSize(first, second);
	}
}
