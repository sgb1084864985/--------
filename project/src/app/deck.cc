#include "deck.h"
#include "wx/dc.h"
#include "wx/dcclient.h"
#include "global.h"
#include <iostream>
#define show_card(card_set) for (int i = 0; i < card_set.GetNumOfCards(); i++) {cerr << card_set.GetCard(i).GetID() << " ";} cerr << endl;

bool initialized = false;
wxBitmap *card_pics[60];

/**
 * @brief Rescale the card image to proper size
 * 
 * @param bitmap bitmap to rescale
 * @param new_size desired size
 * @return wxBitmap* null if bitmap is not valid
 */
wxBitmap *RescaleBitmap (const wxBitmap& bitmap, const wxSize& new_size = k_card_size) {
	if (!bitmap.IsOk()) {
		return nullptr;
	}
	const wxImage image = bitmap.ConvertToImage();
	return new wxBitmap(image.Scale(new_size.GetWidth(), new_size.GetHeight(), wxIMAGE_QUALITY_HIGH));
}

int getCardID(int suit, int rank) {
	switch (suit) {
		case club:
			return rank;
		case diamond:
			return 13 + rank;
		case spade:
			return 26 + rank;
		case heart:
			return 39 + rank;
		case joker:
			return rank == 0 ? 54 : 53;
	}
}

vector<int> suit_order{heart, diamond, club, spade};
vector<int> rank_order{3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, A, 2, R, B};

void Draw(const CardSet& cards, wxDC &dc, int x, int y, CardFace face, CardOrientation orientation, bool is_drawn[]) {
	int n = cards.GetNumOfCards();

	vector<int> card_id;
	for (const int rank : rank_order) {
		if (rank != R && rank != B) {
			for (const int suit : suit_order) {
				for (int i = 0; i < cards.GetNum(suit, rank); i++) {
					card_id.push_back(getCardID(suit, rank));
				}
			}
		} else {
			for (int i = 0; i < cards.GetNum(joker, rank); i++) {
				card_id.push_back(getCardID(joker, rank));
			}
		}
	}

	for (int i = 0; i < n; i++) {
		int draw_y = y;
		if (is_drawn[i] && orientation == kDown) {
			draw_y -= k_delta_y;
		}
		switch (face) {
			case kFaceUp   :
				dc.DrawBitmap(*card_pics[card_id[i]], x, draw_y);
				break;
			case kFaceDown :
				dc.DrawBitmap(*card_pics[0], x, draw_y);
				break;
		}
		switch (orientation) {
			case kLeft:
			case kRight:
				y += k_delta_x;
				break;
			case kUp:
			case kDown:
			case kCenter:
				x += k_delta_y;
				break;
		}
	}
}

BEGIN_EVENT_TABLE(DeckPanel, wxPanel)
	EVT_PAINT(DeckPanel::OnRender)
	EVT_LEFT_UP(DeckPanel::OnClick)
END_EVENT_TABLE()

DeckPanel::DeckPanel (wxWindow *p_parent, CardFace face, CardOrientation orient)
	: wxPanel(p_parent), p_parent(p_parent), face(face), orientation(orient), is_thinking(false), bid(-1) {
	if (initialized == false) {
		wxString prefix("static/Poke/");
		wxString suffix(".jpg");
		try {
			for (int i = 0; i < 55; i++) {
				wxString infix("");
				infix << i;
				auto new_map = new wxBitmap(prefix + infix + suffix, wxBITMAP_TYPE_JPEG);
				card_pics[i] = RescaleBitmap(*new_map);
				delete new_map;
			}
			initialized = true;
		} catch (...) {
			wxMessageBox(wxT("图片未找到！"));
			// Image file lost
		}
	}
	info = new MyLabel(this, wxID_ANY, wxT("正在思考..."));
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(info, 0, wxALIGN_CENTER);
	SetSizer(sizer);
}

CardSet DeckPanel::GetDrawnDeck() {
	vector<int> suits, ranks;
	for (const int rank : rank_order) {
		if (rank != R && rank != B) {
			for (const int suit : suit_order) {
				for (int i = 0; i < card_set.GetNum(suit, rank); i++) {
					suits.push_back(suit);
					ranks.push_back(rank);
				}
			}
		} else {
			for (int i = 0; i < card_set.GetNum(joker, rank); i++) {
				suits.push_back(joker);
				ranks.push_back(rank);
			}
		}
	}

	CardSet result(0);
	int n = card_set.GetNumOfCards();
	for (int i = 0; i < n; i++) {
		if (is_draw[i]) {
			result.Insert(Card(suits[i], ranks[i]));
		}
	}
	return result;
}

CardSet& DeckPanel::GetCardSet() {
	return card_set;
}

void DeckPanel::Release() {
	for (int i = 0; i < card_set.GetNumOfCards(); i++) {
		is_draw[i] = false;
	}
}

void DeckPanel::SetDeck(const CardSet& card_set) {
	this->card_set = card_set;
}

void DeckPanel::SetThinking(bool is_thinking) {
	this->is_thinking = is_thinking;
}

void DeckPanel::SetBidding(int bid) {
	this->bid = bid;
}

void DeckPanel::Render() {
	wxClientDC dc(this);
	int width, height;
	this->GetClientSize(&width, &height);

	if (is_thinking) {
		dc.SetPen(invisible_pen);
		dc.SetBrush(invisible_brush);
		dc.DrawRectangle(wxPoint(0, 0), GetSize());
		info->SetLabelText(wxT("正在思考..."));
		info->Show();
		Layout();
	} else if (bid != -1) {
		wxString info_string("");
		if (bid == 0) {
			info_string << wxT("不叫！");
		} else {
			info_string << wxT("叫地主：") << bid << wxT("分");
		}
		info->SetLabelText(info_string);
		info->Show();
		Layout();
	} else {
		info->Hide();
		Layout();
		int x, y;	// position of the deck
		switch (orientation) {
			case kUp:
				x = 2 * k_card_width;
				y = 0;
				break;
			case kDown:
				x = 2 * k_card_width;
				y = height - k_card_height;
				break;
			case kLeft:
				x = 0;
				y = 0;
				break;
			case kRight:
				x = width - k_card_width;
				y = 0;
				break;
			case kCenter:
				x = width / 2 - (card_set.GetNumOfCards() * k_delta_x + k_card_width) / 2;
				y = height / 2 - k_card_height / 2;
				break;		
		}
		// Draw an invisible rectangle to cover the region
		dc.SetPen(invisible_pen);
		dc.SetBrush(invisible_brush);
		dc.DrawRectangle(wxPoint(0, 0), GetSize());
		Draw(card_set, dc, x, y, face, orientation, is_draw);
	}
}

void DeckPanel::OnRender(wxPaintEvent &event) {
	Render();
	event.Skip();
}

void DeckPanel::OnClick(wxMouseEvent &event) {
	int mouse_x = event.GetPosition().x;
	int mouse_y = event.GetPosition().y;

	if (orientation == kDown) {
		int number_of_cards = card_set.GetNumOfCards();
		int x, y;
		GetClientSize(nullptr, &y);
		
		x = 2 * k_card_width;
		y -= k_card_height;
		for (int i = 0; i < number_of_cards; i++) {
			if (x < mouse_x && (x + k_delta_x > mouse_x || (i == number_of_cards - 1 && x + k_card_width > mouse_x))) {
				if (is_draw[i]) {
					if (y - k_delta_y < mouse_y && mouse_y < y - k_delta_y + k_card_height) {
						is_draw[i] = false;
						break;
					}
				} else {
					if (y < mouse_y && mouse_y < y + k_card_height) {
						is_draw[i] = true;
						break;
					}
				}
			}
			x += k_delta_x;
		}
		Render();
	}
	event.Skip();
}