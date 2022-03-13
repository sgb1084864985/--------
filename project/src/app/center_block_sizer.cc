#include "center_block_sizer.h"

CenterBlockSizer::CenterBlockSizer (wxWindow *p_parent)
	: wxBoxSizer(wxVERTICAL), p_parent(p_parent) {
	panel_box = new wxBoxSizer(wxVERTICAL);
	hbox = new wxBoxSizer(wxHORIZONTAL);
}

void CenterBlockSizer::AddWidget (wxWindow* widget, bool new_line, int alignment) {
	static wxBoxSizer *cur_sizer = nullptr;
	if (new_line) {
		cur_sizer = new wxBoxSizer(wxHORIZONTAL);
		hboxes.push_back(cur_sizer);
	}

	cur_sizer->Add(widget, 1, alignment | wxALL, 5);
}

void CenterBlockSizer::Create () {
	for (auto &sizer : hboxes) {
		panel_box->Add(sizer, 0, wxEXPAND);
	}

	Add(new wxPanel(p_parent), 1, wxALL, 20);
	Add(panel_box, 1, wxALIGN_CENTRE | wxALL, 20);
	Add(new wxPanel(p_parent), 1, wxALL, 20);
}