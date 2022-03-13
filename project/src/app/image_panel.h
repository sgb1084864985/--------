#pragma once
#include <wx/wx.h>
#include <wx/sizer.h>

class wxImagePanel : public wxPanel {
	wxBitmap image;
	
public:
	wxImagePanel(wxWindow* parent, wxString file, wxBitmapType format);
	
	void paintEvent(wxPaintEvent & evt);
	void paintNow();
	void render(wxDC& dc);

	DECLARE_EVENT_TABLE();
	
};