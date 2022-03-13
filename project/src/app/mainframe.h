#pragma once
#include "wx/menu.h"
#include "wx/frame.h"
#include "page_control.h"
#include "global.h"

class MainFrame : public wxFrame {
public:
	MainFrame () = delete;
	MainFrame (const wxString& title);

	PageController *m_controller;	// 页面选择器
	wxMenuBar *menu_bar; 			// 菜单栏
	wxMenu *file;					// 文件菜单

private:
	void OnQuit(wxCommandEvent& event);
};