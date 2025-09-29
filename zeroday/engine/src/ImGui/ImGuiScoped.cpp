#include "ImGui/ImGuiScoped.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Core/Logger.h"
#include "Core/Debug.h"
#define DEBUG_PTR(ptr) DEBUG::DebugForEngineObjectPointers(ptr)

namespace ENGINE::UI
{

	/* *********************************** SCOPED WINDOW ************************************ */

	ImGuiScopedWindow::ImGuiScopedWindow(const char* name, bool* p_open, ImGuiWindowFlags flags)
		: m_open(ImGui::Begin(name, p_open, flags)) {}

	ImGuiScopedWindow::~ImGuiScopedWindow()
	{
		if (m_open)
			ImGui::End();
	}

	/* *********************************** SCOPED TABLE ************************************ */

	ImGuiScopedTable::ImGuiScopedTable(const char* tableName, uint16_t columns, ImGuiWindowFlags flags)
		: m_open(ImGui::BeginTable(tableName, columns, flags)) {}

	ImGuiScopedTable::~ImGuiScopedTable()
	{
		if (m_open)
			ImGui::EndTable();
	}

	/* *********************************** SCOPED MENU ************************************ */

	ImGuiScopedMainMenuBar::ImGuiScopedMainMenuBar()
		: m_open(ImGui::BeginMainMenuBar()) {
	}

	ImGuiScopedMainMenuBar::~ImGuiScopedMainMenuBar()
	{
		if (m_open)
			ImGui::EndMainMenuBar();
	}

	/* *********************************** SCOPED MAIN MENUBAR ************************************ */


	ImGuiScopedMenu::ImGuiScopedMenu(const char* label, bool enabled)
		: m_open(ImGui::BeginMenu(label, enabled))
	{

	}

	ImGuiScopedMenu::~ImGuiScopedMenu()
	{
		if (m_open)
			ImGui::EndMenu();
	}

	/* *********************************** SCOPED POPUP MENU ************************************ */


	ImGuiScopedPopupMenu::ImGuiScopedPopupMenu(const char* label, bool enabled)
		:m_open(ImGui::BeginPopup(label, enabled)) {}

	ImGuiScopedPopupMenu::~ImGuiScopedPopupMenu()
	{
		if (m_open)
			ImGui::EndPopup();
	}

	/* *********************************** SCOPED POPUP MENU ************************************ */


	ImGuiScopedChild::ImGuiScopedChild(const char* label, ImVec2 size, ImGuiChildFlags flags)
		:m_open(ImGui::BeginChild(label, ImVec2(100, 50), flags)) {}

	ImGuiScopedChild::~ImGuiScopedChild()
	{
		if (m_open)
			ImGui::EndChild();
	}

	/* *********************************** SCOPED POPUP MENU ************************************ */


	ImGuiScopedStyleVar::ImGuiScopedStyleVar(ImGuiStyleVar var, ImVec2 fontSize)
	{
		ImGui::PushStyleVar(var, fontSize);
	}

	ImGuiScopedStyleVar::~ImGuiScopedStyleVar()
	{
		ImGui::PopStyleVar();
	}

}