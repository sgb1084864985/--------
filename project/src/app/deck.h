#pragma once
#include "card.h"
#include "wx/bitmap.h"
#include "wx/dcclient.h"
#include "wrapper.h"
#include "global.h"

const int k_card_width = 70;
const int k_card_height = 100;
const int kMaxCards = 30;
const int k_delta_x = 20;
const int k_delta_y = 20;

const wxSize k_card_size(k_card_width, k_card_height);

enum CardOrientation {
	kLeft, kRight, kUp, kDown, kCenter
};

enum CardFace {
	kFaceUp, kFaceDown
};

void Draw(const CardSet& card_set, wxDC &dc, int x, int y, CardFace face, CardOrientation orientation, bool is_drawn[]);

class DeckPanel : public wxPanel {
	CardFace face;					// face up or face down
	CardOrientation orientation;	// the position of the cardset

	bool	 is_thinking;
	int		 bid;
	wxWindow *p_parent;
	MyLabel	 *info;
public:
	CardSet	 card_set;
	bool is_draw[60];				// whether the card has been drawn

	void SetThinking(bool);
	void SetBidding(int);
	CardSet& GetCardSet();
	void Release();
	void OnPaint();
	void OnClick();

public:
	DeckPanel () = delete;
	DeckPanel(wxWindow *p_parent, CardFace face, CardOrientation orient);

	CardSet GetDrawnDeck();

	void SetDeck(const CardSet& card_set);
	void Render();

	void OnRender(wxPaintEvent &event);
	void OnClick(wxMouseEvent &event);

	DECLARE_EVENT_TABLE();
};