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

	bool Engine::run() {
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

	bool Engine::initResources() {
		// Load materials
		if (!g_MaterialLibrary->createMaterials(MATERIAL_JSON_PATH)) {
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
        m_Window = std::make_unique<Window>();

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
		m_World = std::make_unique<ecs::World>();

		Logger::Info("[Engine::InitWorld] World initialized successfully!");
	}

	void Engine::InitOpenGLBufferState() {
		m_GLBufferManager = std::make_unique<opengl::BufferManager>();

		Logger::Info("[Engine::InitOpenGLBufferState] glBufferManager initialized successfully!");
	}

	void Engine::InitAssetManager() {
		g_AssetManager = std::make_unique<ASSET::AssetManager>();

		Logger::Info("[Engine::InitAssetManager] AssetManager initialized successfully!");
	}

	void Engine::InitTexture()
	{
		g_TextureManager = std::make_unique<Zeroday::TextureManager>();

		Logger::Info("[Engine::InitTexture] TextureManager initialized successfully!");
	}

	void Engine::InitMaterial()
	{
		g_MaterialLibrary = std::make_unique<Zeroday::MaterialLibrary>();

		Logger::Info("[Engine::InitMaterial] MaterialLibrary initialized successfully!");
	}

	void Engine::InitMesh()
	{
		g_MeshLibrary = std::make_unique<Zeroday::MeshLibrary>();

		Logger::Info("[Engine::InitMesh] MeshLibrary initialized successfully!");
	}

	void Engine::InitModel()
	{
		g_ModelLoader = std::make_unique<Zeroday::ModelLoader>();

		Logger::Info("[Engine::InitModel] Model Loader initialized successfully!]");
	}

	void Engine::InitRender()
	{
		g_RenderContext = std::make_unique<opengl::RenderContext>();

		Logger::Info("[Engine::InitRender] RenderContext initialized successfully!");

		m_Renderer = std::make_unique<opengl::Renderer>(m_World.get());

		Logger::Info("[Engine::InitRender] RendererManager initialized successfully!");
	}

	void Engine::InitScene()
	{
		m_SceneObjectFactory = std::make_unique<Zeroday::SceneObjectFactory>(*m_World);

		Logger::Info("SceneObjectFactory initialized successfully!");
	}

	void Engine::InitEditor() {
		m_EditorState = std::make_unique<EDITOR::EditorState>();

		const ecs::Entity editorCamera = m_SceneObjectFactory->CreateCamera(ecs::CameraMode::Orbit, ecs::PERSPECTIVE);
		m_World->GetComponent<ecs::NameComponent>(editorCamera)->name = "Editor Camera";

		m_EditorState->cameraEntity = editorCamera;
		m_EditorState->world = m_World.get();

		m_Editor = std::make_unique<EDITOR::Editor>(m_EditorState.get());
		// need update !!?
		m_Editor->addPanel(std::make_unique<EDITOR::UI::InspectorPanel>());
		m_Editor->addPanel(std::make_unique<EDITOR::UI::SceneHierarchyPanel>());
		m_Editor->addPanel(std::make_unique<EDITOR::UI::MenuBarPanel>(m_SceneObjectFactory.get()));

		Logger::Info("[Engine::InitEditor] Editor initialized successfully!]");
	}

	void Engine::InitImGui()
	{
		m_ImGuiLayer = std::make_unique<ENGINE::UI::ImGuiLayer>(m_Window->getGLFWwindow(), m_SceneObjectFactory.get());

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
		m_Renderer->Render();
	}

	void Engine::CleanupPhase() {
		m_World->CleanResources();
		glfwRenderEventStuff();
	}
}
