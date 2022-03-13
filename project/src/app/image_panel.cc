#include "image_panel.h"
#include "wx/image.h"
#include "wx/imagbmp.h"
#include "wx/dc.h"
#include <iostream>


BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
	EVT_PAINT(wxImagePanel::paintEvent)
END_EVENT_TABLE()

wxImagePanel::wxImagePanel(wxWindow* parent, wxString file, wxBitmapType format)
	: wxPanel(parent) {
    image.LoadFile(file, format);
}

void wxImagePanel::paintEvent(wxPaintEvent & evt) {
    wxPaintDC dc(this);
    render(dc);
}

void wxImagePanel::paintNow() {
    wxClientDC dc(this);
    render(dc);
}

void wxImagePanel::render(wxDC&  dc) {
	int height, width;
	GetSize(&width, &height);
	int img_height = image.GetScaledHeight(), img_width = image.GetScaledWidth();
    dc.DrawBitmap(image, (width - img_width) / 2, (height - img_height) / 2, false);
}
