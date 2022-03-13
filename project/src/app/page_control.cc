#include "page_control.h"
#include "page_control_event.h"
#include "events.h"
#include <process.h>

enum {
	kMainMenu = 0,
	kSingleGameMenu,
	kSingleGameJoinMenu,
	kSingleGameCreateMenu,
	kMultiGameMenu,
	kMultiGameJoinSetting,
	kMultiGameCreateSetting,
	kGamePending,
	kGameOver,
	kGameInterface,
};

wxBEGIN_EVENT_TABLE(PageController, wxSimplebook)
	EVT_BUTTON(mainID_play_single, PageController::OnButton)
	EVT_BUTTON(mainID_play_multi, PageController::OnButton)
	EVT_BUTTON(singleID_join, PageController::OnButton)
	EVT_BUTTON(singleID_create, PageController::OnButton)
	EVT_BUTTON(singleID_back, PageController::OnButton)
	EVT_BUTTON(singleCreateID_confirm, PageController::OnButton)
	EVT_BUTTON(singleCreateID_back, PageController::OnButton)
	EVT_BUTTON(singleJoinID_confirm, PageController::OnButton)
	EVT_BUTTON(singleJoinID_back, PageController::OnButton)
	EVT_BUTTON(multiID_join_game, PageController::OnButton)
	EVT_BUTTON(multiID_create_game, PageController::OnButton)
	EVT_BUTTON(multiID_back, PageController::OnButton)
	EVT_BUTTON(joinID_confirm, PageController::OnButton)
	EVT_BUTTON(joinID_back, PageController::OnButton)
	EVT_BUTTON(createID_confirm, PageController::OnButton)
	EVT_BUTTON(createID_back, PageController::OnButton)
	EVT_BUTTON(overID_back, PageController::OnButton)
	EVT_BUTTON(wxID_EXIT, PageController::OnQuit)
	EVT_COMMAND(eventID_game_over, GameOverEvent, PageController::OnGameOver)
	EVT_COMMAND(eventID_join_success, JoinSuccessEvent, PageController::OnJoinSuccess)
	EVT_COMMAND(eventID_join_fail, JoinFailEvent, PageController::OnJoinFail)
	EVT_COMMAND(eventID_create_success, CreateSuccessEvent, PageController::OnCreateSuccess)
	EVT_COMMAND(eventID_create_fail, CreateFailEvent, PageController::OnCreateFail)
wxEND_EVENT_TABLE()

PageController::PageController(wxWindow* p_parent)
	: wxSimplebook(p_parent) {
	main_menu = new MainMenu(this);
	AddPage(main_menu, wxT("Main Menu"), true);

	single_game_menu = new SingleGameMenu(this);
	AddPage(single_game_menu, wxT("Single Game Menu"));

	single_game_join_menu = new SingleGameJoinMenu(this);
	AddPage(single_game_join_menu, wxT("Single Game Join"));

	single_game_create_menu = new SingleGameCreateMenu(this);
	AddPage(single_game_create_menu, wxT("Single Game Create"));

	multi_game_menu = new MultiGameMenu(this);
	AddPage(multi_game_menu, wxT("Multi Game Menu"));

	multi_game_join_setting = new MultiGameJoinSetting(this);
	AddPage(multi_game_join_setting, wxT("Multi Game Join Menu"));

	multi_game_create_setting = new MultiGameCreateSetting(this);
	AddPage(multi_game_create_setting, wxT("Create Game"));

	game_pending = new GamePending(this);
	AddPage(game_pending, wxT("Pending"));

	game_over = new GameOver(this);
	AddPage(game_over, wxT("Over"));

	game_interface = new GameInterface(this);
	AddPage(game_interface, wxT("Game Interface"));
}

void PageController::OnGameOver(wxCommandEvent &event) {
	client.QuitRoom();
	game_over->Render();
	ChangeSelection(kGameOver);
}

void PageController::OnQuit(wxCommandEvent& event) {
	event.Skip();
}

// 菜单系统
void PageController::OnButton(wxCommandEvent& event) {
	switch (event.GetId()) {
		case mainID_play_single:
			ChangeSelection(kSingleGameMenu);
			break;
		case mainID_play_multi:
			ChangeSelection(kMultiGameMenu);
			break;
		case singleID_join:
			ChangeSelection(kSingleGameJoinMenu);
			break;
		case singleID_create:
			ChangeSelection(kSingleGameCreateMenu);
			break;
		case singleID_back:
			ChangeSelection(kMainMenu);
			break;
		case singleJoinID_confirm:
			JoinGame();
			break;
		case singleJoinID_back:
			ChangeSelection(kSingleGameMenu);
			break;
		case singleCreateID_confirm:
			// 单人游戏
			CreateAndJoinGame();
			break;
		case singleCreateID_back:
			ChangeSelection(kSingleGameMenu);
			break;
		case multiID_join_game:
			ChangeSelection(kMultiGameJoinSetting);
			break;
		case multiID_create_game:
			ChangeSelection(kMultiGameCreateSetting);
			break;
		case createID_confirm:
			// 多人游戏，创建房间并加入
			CreateAndJoinGame();
			break;
		case createID_back:
			ChangeSelection(kMultiGameMenu);
			break;
		case multiID_back:
			ChangeSelection(kMainMenu);
			break;
		case joinID_back:
			ChangeSelection(kMultiGameMenu);
			break;
		case joinID_confirm:
			// 多人游戏，加入房间
			JoinGame();
			break;
		case overID_back:
			ChangeSelection(kMainMenu);
			break;
		default:
			event.Skip();
			break;
	}

}

#include "Client.h"
#include "message.h"
#include "ConnectBase.h"
#include "pages.h"
#include <thread>

void GameThread(Client &client, GameInterface *game_interface);
void CreateGameAndJoinThread (AppStatus &status, GameLauncher& launcher, Client& client, PageController* controller);
void JoinGameThread (AppStatus &status, Client& client, PageController *controller);

Client* PageController::GetClient() {
	return &client;
}

void PageController::OnJoinSuccess(wxCommandEvent& event) {
	game_pending->StopPending();
	game_interface->StartGame(client);
	ChangeSelection(kGameInterface);
}

void PageController::OnJoinFail(wxCommandEvent& event) {
	game_pending->StopPending();
	wxMessageBox(wxT("加入游戏失败！连接超时！"));
	ChangeSelection(kMainMenu);
}

void PageController::OnCreateSuccess(wxCommandEvent& event) {
	game_pending->StartPending(GamePending::Status::kJoining);
}

void PageController::OnCreateFail(wxCommandEvent& event) {
	game_pending->StopPending();
	wxMessageBox(wxT("创建房间失败！"));
	ChangeSelection(kMainMenu);
}

void PageController::CreateAndJoinGame() {
	game_pending->StartPending(GamePending::Status::kWaiting);
	ChangeSelection(kGamePending);
	std::thread t(CreateGameAndJoinThread, std::ref(app_status), std::ref(game_launcher), std::ref(client), this);
	t.detach();
}

void PageController::JoinGame() {
	game_pending->StartPending(GamePending::Status::kJoining);
	ChangeSelection(kGamePending);

	std::thread t(JoinGameThread, std::ref(app_status), std::ref(client), this);
	t.detach();
}