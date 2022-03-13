#include "wx/sizer.h"
#include "wx/panel.h"
#include <vector>

class CenterBlockSizer : public wxBoxSizer {
public:
	enum Alignment {
		kCenter = 0,
		kLeft,
		kRight
	};
	CenterBlockSizer(wxWindow* p_prent);
	void AddWidget(wxWindow* widget, bool new_line = false, int alignment = wxALIGN_LEFT);
	void Create();

private:
	wxWindow 	*p_parent;
	wxBoxSizer	*panel_box, *hbox;
	std::vector <wxBoxSizer*> hboxes;
};