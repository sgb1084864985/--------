#pragma once
#include "wx/panel.h"
#include "wx/font.h"
#include "wx/colour.h"
#include "wx/pen.h"
#include "wx/brush.h"
#include "wx/settings.h"

const int screen_width = wxSystemSettings::GetMetric(wxSYS_SCREEN_X, NULL);
const int screen_height = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y, NULL);

const int k_window_length = screen_width;
const int k_window_height = screen_height;

const wxPoint k_default_position(50, 50);
const wxSize k_default_size(k_window_length, k_window_height);

const wxColour	k_bgcolor("#EFEFEF");
const wxPen		invisible_pen(k_bgcolor);
const wxBrush	invisible_brush(k_bgcolor);
const wxFont	h1_font(30, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
const wxFont	button_font(18, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
const wxFont	label_font(18, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
