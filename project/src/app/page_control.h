#ifndef PAGE_CONTROL_H
#define PAGE_CONTROL_H
#define WIN32_LEAN_AND_MEAN
#include "wx/simplebook.h"
#include "Client.h"
#include "GameLauncher.h"
#include "pages.h"

enum GUISingleOrMulti {
	kSingle = 0,
	kMulti
};

enum GUIGameType {
	kLandlord3 = 0,
	kLandlord4,
	k2v2
};

enum GUIJoinOrCreate {
	kJoin,
	kCreate
};

struct AppStatus {
	GUISingleOrMulti single_multi;
	GUIGameType	game_type;
	GUIJoinOrCreate join_create;
	wxString user_name;
	wxString IP_address;
	wxString passwd;
	int player_number;
};

class PageController : public wxSimplebook {
public:
	AppStatus app_status;
	GameLauncher game_launcher;
	Client client;

	PageController () = delete;
	PageController (wxWindow* p_parent);
	MainMenu				*main_menu;
	SingleGameMenu			*single_game_menu;
	SingleGameCreateMenu	*single_game_create_menu;
	SingleGameJoinMenu		*single_game_join_menu;
	MultiGameMenu			*multi_game_menu;
	MultiGameJoinSetting	*multi_game_join_setting;
	MultiGameCreateSetting	*multi_game_create_setting;
	GamePending				*game_pending;
	GameOver				*game_over;
	GameInterface			*game_interface;

	void OnQuit(wxCommandEvent& event);
	void OnButton(wxCommandEvent& event);

	void OnGameOver(wxCommandEvent &event);
	void OnJoinSuccess(wxCommandEvent& event);
	void OnJoinFail(wxCommandEvent& event);
	void OnCreateSuccess(wxCommandEvent& event);
	void OnCreateFail(wxCommandEvent& event);

	void JoinGame();
	void CreateAndJoinGame();
	Client *GetClient();

	wxDECLARE_EVENT_TABLE();

};

#endif