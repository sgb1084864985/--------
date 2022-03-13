#pragma once
#include "global.h"
#include "wx/button.h"
#include "wx/colour.h"
#include "wx/wx.h"

class MyButton : public wxButton
{
public:
	MyButton(wxWindow *parent,
			 wxWindowID id,
			 const wxString &label = wxEmptyString,
			 const wxPoint &pos = wxDefaultPosition,
			 const wxSize &size = wxDefaultSize,
			 long style = 0,
			 const wxValidator &validator = wxDefaultValidator,
			 const wxString &name = wxButtonNameStr);
};

class MyLabel : public wxStaticText
{
public:
	MyLabel(wxWindow *parent,
			wxWindowID id,
			const wxString &label,
			const wxPoint &pos = wxDefaultPosition,
			const wxSize &size = wxDefaultSize,
			long style = 0,
			const wxString &name = wxStaticTextNameStr);
};