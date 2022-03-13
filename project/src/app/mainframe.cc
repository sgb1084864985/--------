#include "mainframe.h"
#include "image_panel.h"

MainFrame::MainFrame(const wxString& title)
	: wxFrame(nullptr, wxID_ANY, title, k_default_position, k_default_size) {

	// Add Menu 
	menu_bar = new wxMenuBar;
	file = new wxMenu;
	file->Append(wxID_EXIT, wxT("&Quit"));

	menu_bar->Append(file, wxT("&File"));
	SetMenuBar(menu_bar);

	// Add Page Controller
	m_controller = new PageController(this);

	this->SetMinSize(k_default_size);
	this->SetMaxSize(k_default_size);

	// Change background color
	// wxColor bg_color;
	// bg_color.Set(wxT("#ffffff"));
	// SetBackgroundColour(bg_color);

	Connect(wxID_EXIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(OnQuit));
	Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(OnQuit));
	Center();
}

void MainFrame::OnQuit(wxCommandEvent& event) {
	Close(true);
}