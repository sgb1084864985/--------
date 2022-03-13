#ifndef PAGES_H
#define PAGES_H

#include "wx/panel.h"
#include "wx/textctrl.h"
#include "wx/textwrapper.h"
#include "wx/spinctrl.h"
#include "wx/choice.h"
#include "image_panel.h"
#include "deck.h"
#include "wrapper.h"
#include "message.h"
#include "Client.h"

class MainMenu : public wxPanel {
public:
	MainMenu () = delete;
	MainMenu (wxWindow* p_parent);

	wxWindow 	 *p_parent;
	wxPanel 	 *right_panel;
	wxImagePanel *image;
	MyLabel 	 *title;
	MyButton 	 *b_play_single;
	MyButton 	 *b_play_multi;
	MyButton 	 *b_quit;

	void OnPlaySingle(wxCommandEvent& event);
	void OnPlayMulti(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class SingleGameMenu : public wxPanel {
public:
	SingleGameMenu () = delete;
	SingleGameMenu (wxWindow *p_parent);

	MyLabel			*title;
	wxWindow		*p_parent;
	MyButton		*join;
	MyButton		*create;
	MyButton		*go_back;

	void OnCreate(wxCommandEvent &event);
	void OnJoin(wxCommandEvent &event);
	void OnReturn(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

class SingleGameCreateMenu : public wxPanel {
public:
	SingleGameCreateMenu () = delete;
	SingleGameCreateMenu (wxWindow *p_parent);

	MyLabel 		*title;
	wxWindow 	 	*p_parent;
	MyLabel 		*game_select_label;
	wxChoice		*game_select;
	MyLabel 		*user_name_label;
	wxTextCtrl   	*user_name_input;
	MyLabel 		*user_number_label;
	wxSpinCtrl   	*user_number_input;
	MyButton		*confirm;
	MyButton		*go_back;

	void OnConfirm(wxCommandEvent &event);
	void OnReturn(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

class SingleGameJoinMenu : public wxPanel {
public:
	SingleGameJoinMenu () = delete;
	SingleGameJoinMenu (wxWindow *p_parent);

	MyLabel		*title;
	wxWindow	*p_parent;
	MyLabel		*user_name_label;
	wxTextCtrl	*user_name_input;
	MyButton	*confirm;
	MyButton	*go_back;

	void OnConfirm(wxCommandEvent& event);
	void OnReturn(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class MultiGameMenu : public wxPanel {
public:
	MultiGameMenu () = delete;
	MultiGameMenu (wxWindow *p_parent);

	wxWindow		*p_parent;
	MyLabel			*title;
	MyButton		*join_game;
	MyButton		*create_game;
	MyButton		*go_back;

	void OnJoin(wxCommandEvent &event);
	void OnCreate(wxCommandEvent &event);
	void OnReturn(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

class MultiGameJoinSetting : public wxPanel {
public:
	MultiGameJoinSetting () = delete;
	MultiGameJoinSetting (wxWindow *p_parent);
	
	wxWindow		*p_parent;
	MyLabel			*title;
	MyLabel			*user_name_label;
	wxTextCtrl		*user_name_input;
	MyLabel			*IP_label;
	wxTextCtrl		*IP_input;
	MyButton		*confirm;
	MyButton		*go_back;

	void OnConfirm(wxCommandEvent &event);
	void OnReturn(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

class MultiGameCreateSetting : public wxPanel {
public:
	MultiGameCreateSetting () = delete;
	MultiGameCreateSetting (wxWindow *p_parent);

	wxWindow		*p_parent;
	MyLabel			*title;
	MyLabel 		*game_select_label;
	wxChoice		*game_select;
	MyLabel			*user_name_label;
	wxTextCtrl		*user_name_input;
	MyLabel 		*user_number_label;
	wxSpinCtrl   	*user_number_input;
	MyButton		*confirm;
	MyButton		*go_back;

	void OnConfirm(wxCommandEvent &event);
	void OnReturn(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

class GamePending : public wxPanel {
public:
	GamePending () = delete;
	GamePending (wxWindow *p_parent);
	
	enum Status {
		kWaiting,
		kJoining
	} status;

	int			count;
	wxWindow	*p_parent;
	wxTimer		*timer;
	MyLabel		*wait_label;

	void StartPending(Status status);
	void StopPending();
	void OnTimer(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

class GameOver : public wxPanel {
	wxWindow	*p_parent;

	MyLabel		*title;
	MyLabel		*score_label[4];
	wxButton	*go_back;

public:
	std::string	user_name[4];
	int			score[4];
	int			num_player;

	GameOver () = delete;
	GameOver (wxWindow *p_parent);

	void Render();
	void SetScore(std::string user_name[], int score[], int num_player);
	void OnReturn(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
};

class GameInterface : public wxPanel {
public:
	GameInterface () = delete;
	GameInterface (wxWindow* p_parent);

	wxTimer			*timer;
	MyLabel			*center_info;
	MyLabel			*stake_info;
	MyLabel			*timer_info;
	MyLabel			*user_info[4];
	wxWindow		*p_parent;
	MyButton		*deal;
	MyButton		*pass;
	MyButton		*call_auction[4];
	wxClientDC		dc;
	DeckPanel		*deck[4];
	DeckPanel		*last_round[4];
	wxPanel			*midpan;

	void StartGame(Client &client);
	void ChangeMyCards(const CardSet &deck);

	void OnDenied(wxCommandEvent &event);
	void OnLogOut(wxCommandEvent &event);
	void OnBid(wxCommandEvent &event);
	void OnDeal(wxCommandEvent &event);
	void OnPass(wxCommandEvent &event);
	void OnTimer(wxTimerEvent &event);
	void OnRefresh(wxCommandEvent &event);
	void OnGameOver(wxCommandEvent &event);
	void Render();

	wxDECLARE_EVENT_TABLE();
};

#endif