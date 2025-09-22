#include "core/Engine.h"
#include "core/Config.h"
#include "CallBack/CallBack.h"
#include <imgui_impl_opengl3.h>
#include <Input/Input.h>
#include "core/Services.h"
#include "Editor/Panel/InspectorPanel.h"
#include "Editor/Panel/MenuBarPanel.h"
#include "Editor/Panel/SceneHierarchyPanel.h"

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
			Logger::Error("[Engine::run] GLFW window returning nullptr!");
			return false;
		}
		glfwSwapInterval(0);

		Logger::Info("Starting engine...");

		glEnable(GL_DEBUG_OUTPUT);

		// engine life loop
		EditorStateLoop(glfwWin);

		Logger::Info("Finishing engine...");
		return true;
	}

	bool Engine::InitResources() {
		// Load materials
		if (!g_MaterialLibrary->CreateMaterials(MATERIAL_JSON_PATH)) {
			Logger::Error("[Engine::initResources] loadMaterialFromJSON function is not working correctly!");
			return false;
		}

		Logger::Info("[Engine::initResources] Materials loaded successfully!");

		// Load shaders
		if (!g_AssetManager->loadAllShaders()) {
			Logger::Error("[Engine::initResources] loadAllShaders FAILED!");
		}

		Logger::Info("[Engine::initResources] Shaders loaded successfully!");

		m_ImGuiLayer->Init(m_Window->getGLFWwindow());

		m_GLBufferManager->Init();

		Logger::Info("Engine initResources successful!");

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
			InitWorld();
			InitOpenGLBufferState();
			InitAssetManager();
			InitTexture();
			InitMaterial();
			InitMesh();
			InitModel();
			InitRender();
			InitScene();
			InitEditor();
			InitImGui();
			InitServices();

			Logger::Info("[Engine::InitPointerObjects] Successful!");
		}
		catch (const std::exception& e) {
			Logger::Error("[Engine::initPointerObjects] FAILED! Error: " + std::string(e.what()));
			throw;
		}
	}

    void Engine::EditorStateLoop(GLFWwindow* glfwWin)
    {
		while (!glfwWindowShouldClose(glfwWin) && !m_EditorState->requestShutdown)
		{
			UpdatePhase();
			RenderPhase();
			UIPhase();
			CleanupPhase();
		}
    }

    void Engine::InitWindow()
    {
        m_Window = CreateScope<Window>();

		if (!m_Window->initResources()) {
			Logger::Error("Engine::initWindow FAILED because of initResources!");
			throw std::runtime_error("Failed to initialize window FAILED because of initResources!");
		}

		Logger::Info("[Engine::InitWindow] Window initialized successfully!]");
    }

    void Engine::InitCallBack() {
		CallBack::initResources(m_Window->getGLFWwindow());

		Logger::Info("[Engine::InitCallBack] initialized successfully!");
	}

	void Engine::InitWorld() {
		m_Scene = CreateScope<ecs::Scene>();

		Logger::Info("[Engine::InitWorld] World initialized successfully!");
	}

	void Engine::InitOpenGLBufferState() {
		m_GLBufferManager = CreateScope<opengl::BufferManager>();

		Logger::Info("[Engine::InitOpenGLBufferState] glBufferManager initialized successfully!");
	}

	void Engine::InitAssetManager() {
		g_AssetManager = CreateScope<ASSET::AssetManager>();

		Logger::Info("[Engine::InitAssetManager] AssetManager initialized successfully!");
	}

	void Engine::InitTexture()
	{
		g_TextureManager = CreateScope<Zeroday::TextureManager>();

		Logger::Info("[Engine::InitTexture] TextureManager initialized successfully!");
	}

	void Engine::InitMaterial()
	{
		g_MaterialLibrary = CreateScope<Zeroday::MaterialLibrary>();

		Logger::Info("[Engine::InitMaterial] MaterialLibrary initialized successfully!");
	}

	void Engine::InitMesh()
	{
		g_MeshLibrary = CreateScope<Zeroday::MeshLibrary>();

		Logger::Info("[Engine::InitMesh] MeshLibrary initialized successfully!");
	}

	void Engine::InitModel()
	{
		g_ModelLoader = CreateScope<Zeroday::ModelLoader>();

		Logger::Info("[Engine::InitModel] Model Loader initialized successfully!]");
	}

	void Engine::InitRender()
	{
		g_RenderContext = CreateScope<opengl::RenderContext>();

		Logger::Info("[Engine::InitRender] RenderContext initialized successfully!");

		m_Renderer3D = CreateScope<opengl::Renderer3D>(m_Scene.get());

		Logger::Info("[Engine::InitRender] RendererManager initialized successfully!");
	}

	void Engine::InitScene()
	{
		m_SceneObjectFactory = CreateScope<Zeroday::SceneObjectFactory>(*m_Scene);

		Logger::Info("SceneObjectFactory initialized successfully!");
	}

	void Engine::InitEditor() {
		m_EditorState = CreateScope<EDITOR::EditorState>();

		const ecs::Entity editorCamera = m_SceneObjectFactory->CreateCamera(ecs::CameraMode::Orbit, ecs::PERSPECTIVE);
		m_Scene->GetComponent<ecs::NameComponent>(editorCamera)->name = "Editor Camera";

		m_EditorState->cameraEntity = editorCamera;
		m_EditorState->world = m_Scene.get();

		m_Editor = CreateScope<EDITOR::Editor>(m_EditorState.get());
		// need update !!?
		m_Editor->addPanel(CreateScope<EDITOR::UI::InspectorPanel>());
		m_Editor->addPanel(CreateScope<EDITOR::UI::SceneHierarchyPanel>());
		m_Editor->addPanel(CreateScope<EDITOR::UI::MenuBarPanel>(m_SceneObjectFactory.get()));

		Logger::Info("[Engine::InitEditor] Editor initialized successfully!]");
	}

	void Engine::InitImGui()
	{
		m_ImGuiLayer = CreateScope<ENGINE::UI::ImGuiLayer>(m_Window->getGLFWwindow(), m_SceneObjectFactory.get());

		Logger::Info("[Engine::InitImGui] ImGuiLayer initialized successfully!");
	}

	void Engine::OpenGLSetUpResources() noexcept
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
	}

	void Engine::OpenGLRenderStuff() noexcept
	{
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
		m_Scene->CleanResources();
		glfwRenderEventStuff();
	}
}
