#pragma once
#include <memory>
#include <unordered_map>
#include "ImGui/ImGuiObjectState.h"


namespace ecs {
	struct CameraComponent;
	struct LightComponent;
	struct MaterialComponent;
	struct NameComponent;
	struct TransformComponent;
	class Scene;
}
namespace Zeroday { class SceneObjectFactory; }
struct ImGuiIO;
struct GLFWwindow;

namespace Zeroday::UI
{
	class ImGuiLayer {
	public:
		ImGuiLayer(GLFWwindow* window, SceneObjectFactory* factory);

		void Init(GLFWwindow* window);
		void BeginFrame();
		void EndFrame();
		void Shutdown();

	private:
		GLFWwindow* m_Window = nullptr;
		Zeroday::SceneObjectFactory* sceneObjectFactory = nullptr;

		bool showDemoWindow = true;
		bool showAnotherWindow = false;

		bool g_RequestShutdown = false;

		std::unordered_map<std::string, UIObjectState> m_objectUIStates;

		bool m_showSceneEditorUI = false;
		bool m_showObjectListWindow   = false;
		bool m_showLightSourceObjects = false;

		std::string m_fontPath{};

		void SetDarkThemeColors();
		void InitFontAndImages(const ImGuiIO& io);
	};
}
