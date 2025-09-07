#pragma once
#include <memory>
#include <unordered_map>
#include "ImGui/ImGuiObjectState.h"


namespace ECS {
	struct CameraComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct NameComponent;
	struct TransformComponent;
	class World;
}
namespace Graphics { class SceneObjectFactory; }
struct ImGuiIO;
struct GLFWwindow;

namespace ENGINE::UI
{
	class ImGuiLayer
	{
	public:
		ImGuiLayer(GLFWwindow* window, Graphics::SceneObjectFactory* factory);

		void Init(GLFWwindow* window);
		void initFontAndImages(const ImGuiIO& io);
		void BeginFrame();
		void EndFrame();
		void Shutdown();

		void SetDarkThemeColors();

	private:
		GLFWwindow* m_Window = nullptr;
		Graphics::SceneObjectFactory* sceneObjectFactory = nullptr;

		bool showDemoWindow = true;
		bool showAnotherWindow = false;

		bool g_RequestShutdown = false;

		std::unordered_map<std::string, UIObjectState> m_objectUIStates;

		bool m_showSceneEditorUI = false;
		bool m_showObjectListWindow   = false;
		bool m_showLightSourceObjects = false;

		std::string m_fontPath{};
	};
}
