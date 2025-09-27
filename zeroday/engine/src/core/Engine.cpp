#include "core/Engine.h"
#include "core/Config.h"
#include "CallBack/CallBack.h"
#include <imgui_impl_opengl3.h>
#include <core/EngineConfig.h>
#include <Input/Input.h>
#include "core/Services.h"
#include "Editor/InspectorPanel.h"
#include "Editor/MenuBarPanel.h"
#include "Editor/SceneHierarchyPanel.h"
#include "Graphics/OpenGL/Mesh/MeshData3D.h"
#include "Scene/SceneObjectFactory.h"

namespace Zeroday {

	Engine::~Engine() {
		if (m_ImGuiLayer) {
			m_ImGuiLayer->Shutdown();
		}
	}

	bool Engine::Run() {
		OpenGLSetUpResources();

		const auto glfwWin = m_Window->getGLFWwindow();

		if (!glfwWin) {
			Error("[Engine::run] GLFW window returning nullptr!");
			return false;
		}
		glfwSwapInterval(0);

		Info("Starting engine...");

		glEnable(GL_DEBUG_OUTPUT);

		// engine life loop
		EditorStateLoop(glfwWin);

		Info("Finishing engine...");
		return true;
	}

	bool Engine::InitResources() {
		// Load materials
		if (!g_MaterialLibrary->CreateMaterials(std::string(MATERIAL_JSON_PATH) + "materials.json")) {
			Error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		Info("[Engine::initResources] Materials loaded successfully!");

		// Load Shaders
		m_Renderer3D->Init();

		Info("[Engine::initResources] Shaders loaded successfully!");

		m_ImGuiLayer->Init(m_Window->getGLFWwindow());

		(void)m_SceneObjectFactory->CreateLight(opengl::LightType::Point, "Point Light");
		(void)m_SceneObjectFactory->CreateLight(opengl::LightType::Directional, "Directional Light");
		(void)m_SceneObjectFactory->CreateLight(opengl::LightType::Spot, "Spot Light");

		(void)m_SceneObjectFactory->CreatePrimitiveObject("cube", "CUBE");
		(void)m_SceneObjectFactory->CreatePrimitiveObject("sphere", "SPHERE");

		Info("Engine initResources successful!");

		return true;
	}

	void Engine::InitServices() {
		Services::RegisterMaterialLibrary(g_MaterialLibrary.get());
		Services::RegisterMeshLibrary(g_MeshLibrary.get());
		Services::RegisterAssetManager(g_AssetManager.get());
		Services::RegisterModelLoader(g_ModelLoader.get());
		Services::RegisterRenderContext(g_RenderContext.get());
		Services::RegisterTextureManager(g_TextureManager.get());
		Services::RegisterEditorState(m_EditorState.get());
	}

	void Engine::InitPointerObjects() {
		try {
			InitWindow();
			InitCallBack();
			InitAssetManager();
			InitTexture();
			InitMaterial();
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

		while (!glfwWindowShouldClose(glfwWin) && !m_EditorState->requestShutdown)
		{
			UpdatePhase();
			RenderPhase();
			UIPhase();
			CleanupPhase();
		}
    }

    void Engine::InitWindow() {
        m_Window = CreateScope<Window>();

		if (!m_Window->initResources()) {
			Error("Engine::initWindow FAILED because of initResources!");
			throw std::runtime_error("Failed to initialize window FAILED because of initResources!");
		}

		Info("[Engine::InitWindow] Window initialized successfully!]");
    }

    void Engine::InitCallBack() {
		CallBack::initResources(m_Window->getGLFWwindow());

		Info("[Engine::InitCallBack] initialized successfully!");
	}

	void Engine::InitAssetManager() {
		g_AssetManager = CreateScope<AssetManager>();

		Info("[Engine::InitAssetManager] AssetManager initialized successfully!");
	}

	void Engine::InitTexture() {
		g_TextureManager = CreateScope<TextureManager>();

		Info("[Engine::InitTexture] TextureManager initialized successfully!");
	}

	void Engine::InitMaterial() {
		g_MaterialLibrary = CreateScope<MaterialLibrary>();

		Info("[Engine::InitMaterial] MaterialLibrary initialized successfully!");
	}

	void Engine::InitMesh() {
		g_MeshLibrary = CreateScope<MeshLibrary>();

		Info("[Engine::InitMesh] MeshLibrary initialized successfully!");
	}

	void Engine::InitModel() {
		g_ModelLoader = CreateScope<ModelLoader>();

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
		// need update !!?
		m_Editor->addPanel(CreateScope<Editor::UI::InspectorPanel>());
		m_Editor->addPanel(CreateScope<Editor::UI::SceneHierarchyPanel>());
		m_Editor->addPanel(CreateScope<Editor::UI::MenuBarPanel>(m_SceneObjectFactory.get()));

		Info("[Engine::InitEditor] Editor initialized successfully!]");
	}

	void Engine::InitImGui() {
		m_ImGuiLayer = CreateScope<UI::ImGuiLayer>(m_Window->getGLFWwindow(), m_SceneObjectFactory.get());

		Info("[Engine::InitImGui] ImGuiLayer initialized successfully!");
	}

	void Engine::OpenGLSetUpResources() noexcept {
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void Engine::OpenGLRenderStuff() noexcept {
		// Screen Color
		glClearColor(0.04f, 0.05f, 0.06f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		while (glGetError() != GL_NO_ERROR);
		// glDisable(GL_CULL_FACE);
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void Engine::glfwRenderEventStuff() const noexcept {
		glfwPollEvents();
		glfwSwapBuffers(m_Window->getGLFWwindow());
	}

	void Engine::UpdatePhase() {
		Input::update();
		// world->update();
	}

	void Engine::UIPhase() {
		m_ImGuiLayer->BeginFrame();
		m_Editor->drawUI();
		m_ImGuiLayer->EndFrame();
	}

	void Engine::RenderPhase() {
		OpenGLRenderStuff();
		m_Renderer3D->Render();
	}

	void Engine::CleanupPhase() {
		glfwRenderEventStuff();
	}
}
