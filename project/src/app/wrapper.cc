#include "wrapper.h"

MyButton::MyButton(wxWindow *parent,
				   wxWindowID id,
				   const wxString &label,
				   const wxPoint &pos,
				   const wxSize &size,
				   long style,
				   const wxValidator &validator,
				   const wxString &name)
	: wxButton(parent, id, label, pos, size, style, validator, name)
{
	SetFont(button_font);
}

MyLabel::MyLabel(wxWindow *parent,
				 wxWindowID id,
				 const wxString &label,
				 const wxPoint &pos,
				 const wxSize &size,
				 long style,
				 const wxString &name) : wxStaticText(parent, id, label, pos, size, style, name)
{
	SetFont(label_font);
}