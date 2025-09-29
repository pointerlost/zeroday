#include "ImGui/ImGuiLayer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Graphics/OpenGL/Lighting/Light.h"
#include "core/Config.h"
#include <Core/File.h>
#include "Core/Logger.h"

namespace Zeroday::UI
{
	ImGuiLayer::ImGuiLayer(GLFWwindow* window, Zeroday::SceneObjectFactory* factory)
		: m_Window(window), sceneObjectFactory(factory) {}

	void ImGuiLayer::Init(GLFWwindow* window)
	{
		m_Window = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

		ImGui::StyleColorsClassic();

		// set the ImGui font style and text size
		initFontAndImages(io);

		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 460");	// according to shader version

		// Set Theme
		SetDarkThemeColors();
	}

	void ImGuiLayer::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::initFontAndImages(const ImGuiIO& io)
	{
		const auto& file = Zeroday::File::Get();

		m_fontPath = std::string(ASSETS_DIR) + "/fonts/RobotoSlab.ttf";

		if (file.Exists(m_fontPath)) {
			io.Fonts->AddFontFromFileTTF(m_fontPath.c_str(), 20.0f);
		}
		else {
			Error("[ImGuiLayer::Init::initFont] Font file not found at path! so Default path assigned!");
			io.Fonts->AddFontDefault();
		}

		// load texture for inspector background
		// m_renderData->getTextureManager()->load("inspectorBG", nlohmann::detail::concat(ASSETS_DIR, "/textures/inspectorBG.png"));
	}

	void ImGuiLayer::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::SetDarkThemeColors() {
		auto &colors = ImGui::GetStyle().Colors;

		// ColorEdit3 widget background
		colors[ImGuiCol_FrameBg]        = ImVec4(0.18f, 0.21f, 0.220f, 1.0f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.28f, 0.27f, 1.0f);
		colors[ImGuiCol_FrameBgActive]  = ImVec4(0.33f, 0.36f, 0.35f, 1.0f);

		// Headers
		colors[ImGuiCol_Header]        = ImVec4(0.28f, 0.32f, 0.31f, 1.0f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.56f, 0.60f, 1.0f);
		colors[ImGuiCol_HeaderActive]  = ImVec4(0.48f, 0.65f, 0.70f, 1.0f);

		// Menu
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.0f);

		// Window Background
		colors[ImGuiCol_WindowBg]  = ImVec4(0.05f, 0.05f, 0.07f, 1.0f);
		colors[ImGuiCol_ChildBg]   = ImVec4(0.07f, 0.07f, 0.09f, 1.0f);

		// Titles
		colors[ImGuiCol_TitleBg]       = ImVec4(0.05f, 0.05f, 0.07f, 1.0f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.15f, 1.0f);

		// Buttons
		colors[ImGuiCol_Button]        = ImVec4(0.20f, 0.25f, 0.30f, 1.0f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.35f, 0.40f, 1.0f);
		colors[ImGuiCol_ButtonActive]  = ImVec4(0.10f, 0.15f, 0.20f, 1.0f);

		// Checkbox Color
		colors[ImGuiCol_CheckMark] =  ImVec4(0.80f, 0.80f, 0.80f, 1.0f);

		colors[ImGuiCol_Border]    = ImVec4(0.2f, 0.25f, 0.27f, 1.0f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
	}

	/********************************** ImGuiScopedWindow *************************************/
}
