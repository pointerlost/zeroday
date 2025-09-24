#pragma once
#include <cstdint>
#include <iostream>
#include <imgui.h>

namespace ENGINE::UI
{
										/* SCOPE is a RAII wrapper */
	
	// These structures are scoped wrappers for ImGui functionality.

	struct ImGuiScopedWindow
	{
	private:
		bool m_open;
	
	public:
		ImGuiScopedWindow(const char* name, bool* p_open = nullptr, ImGuiWindowFlags flags = 0);
		~ImGuiScopedWindow();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED TABLE ************************************ */

	struct ImGuiScopedTable
	{
	private:
		bool m_open;

	public:
		ImGuiScopedTable(const char* id, uint16_t columns, ImGuiWindowFlags flags = 0);
		~ImGuiScopedTable();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED MENU ************************************ */

	struct ImGuiScopedMainMenuBar
	{
	private:
		bool m_open;

	public:
		ImGuiScopedMainMenuBar();
		~ImGuiScopedMainMenuBar();

		operator bool() const { return m_open; };
	};
	
	/* *********************************** SCOPED MENU ************************************ */

	struct ImGuiScopedMenu
	{
	private:
		bool m_open;

	public:
		ImGuiScopedMenu(const char* label, bool enabled = true);
		~ImGuiScopedMenu();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED MENU ************************************ */

	struct ImGuiScopedPopupMenu
	{
	private:
		bool m_open;

	public:
		ImGuiScopedPopupMenu(const char* label, bool enabled = true);
		~ImGuiScopedPopupMenu();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED CHILD ************************************ */

	struct ImGuiScopedChild
	{
	private:
		bool m_open;

	public:
		ImGuiScopedChild(const char* label, ImVec2 size = {150,100}, ImGuiChildFlags flags = ImGuiChildFlags_None);
		~ImGuiScopedChild();

		operator bool() const { return m_open; };
	};

	/* *********************************** SCOPED CHILD ************************************ */

	struct ImGuiScopedStyleVar
	{
	public:
		ImGuiScopedStyleVar(ImGuiStyleVar var, ImVec2 fontSize = { 12,9 });
		~ImGuiScopedStyleVar();
	};
}