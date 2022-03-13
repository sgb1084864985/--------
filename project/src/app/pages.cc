#include <iostream>
#include "pages.h"
#include "wx/button.h"
#include "wx/sizer.h"
#include "wx/spinctrl.h"
#include "wx/dcclient.h"
#include "image_panel.h"
#include "center_block_sizer.h"
#include "page_control_event.h"
#include "deck.h"
#include "page_control.h"
#include "events.h"

#define get_controller(controller) auto (controller) = static_cast<PageController*>(this->p_parent);
#define show_card(card_set) for (int i = 0; i < card_set.GetNumOfCards(); i++) {cerr << card_set.GetCard(i).GetID() << " ";} cerr << endl;

wxBEGIN_EVENT_TABLE(MainMenu, wxPanel)
	EVT_BUTTON(mainID_play_single, MainMenu::OnPlaySingle)
	EVT_BUTTON(mainID_play_multi, MainMenu::OnPlayMulti)
	EVT_BUTTON(wxID_EXIT, MainMenu::OnQuit)
wxEND_EVENT_TABLE()

MainMenu::MainMenu(wxWindow* p_parent)
	: wxPanel(p_parent), p_parent(p_parent) {

	right_panel 	= new wxPanel(this);
	title 			= new MyLabel(right_panel, wxID_ANY, wxT("主菜单"));
	image 			= new wxImagePanel(this, wxString("./static/main.jpg"), wxBITMAP_TYPE_JPEG);
	b_play_single 	= new MyButton(right_panel, mainID_play_single, wxT("本地游戏"));
	b_play_multi	= new MyButton(right_panel, mainID_play_multi, wxT("联机游戏"));
	b_quit 			= new MyButton(right_panel, wxID_EXIT, wxT("退出游戏"));

	title->SetFont(h1_font);
	b_play_single->SetFont(button_font);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	hbox->Add(image, 1, wxEXPAND | wxALL, 20);

	vbox->Add(title, 	   		1, wxALIGN_CENTER_HORIZONTAL | wxUP | wxBOTTOM, 10);
	vbox->Add(b_play_single,  	0, wxALL | wxCENTER, 10);
	vbox->Add(b_play_multi, 	0, wxALL | wxCENTER, 10);
	vbox->Add(b_quit, 	   		0, wxALL | wxCENTER, 10);
	right_panel->SetSizer(vbox);

	hbox->Add(right_panel, 1, wxALIGN_CENTER | wxUP | wxBOTTOM, 10);

	SetSizer(hbox);
}

void MainMenu::OnQuit(wxCommandEvent& event) {
	event.Skip();
}

void MainMenu::OnPlaySingle(wxCommandEvent& event) {
	auto control = static_cast<PageController*>(this->p_parent);
	control->app_status.single_multi = kSingle;
	control->app_status.IP_address = "127.0.0.1";
	event.Skip();
}

void MainMenu::OnPlayMulti(wxCommandEvent& event) {
	auto control = static_cast<PageController*>(this->p_parent);
	control->app_status.single_multi = kMulti;
	event.Skip();
}

wxBEGIN_EVENT_TABLE(SingleGameMenu, wxPanel)
	EVT_BUTTON(singleID_create, SingleGameMenu::OnCreate)
	EVT_BUTTON(singleID_join, SingleGameMenu::OnJoin)
	EVT_BUTTON(singleID_back, SingleGameMenu::OnReturn)
wxEND_EVENT_TABLE()

SingleGameMenu::SingleGameMenu(wxWindow *p_parent)
	: wxPanel(p_parent), p_parent(p_parent) {
	title = new MyLabel(this, wxID_ANY, wxT("本地游戏"));
	join = new MyButton(this, singleID_join, wxT("加入本地游戏"));
	create = new MyButton(this, singleID_create, wxT("创建本地游戏房间"));
	go_back = new MyButton(this, singleID_back, wxT("返回"));

	CenterBlockSizer *sizer = new CenterBlockSizer(this);
	sizer->AddWidget(title, true);
	sizer->AddWidget(join, true);
	sizer->AddWidget(create, true);
	sizer->AddWidget(go_back, true);
	sizer->Create();

	SetSizer(sizer);
}

void SingleGameMenu::OnCreate(wxCommandEvent &event) {
	event.Skip();
}

void SingleGameMenu::OnJoin(wxCommandEvent &event) {
	event.Skip();
}

void SingleGameMenu::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(SingleGameCreateMenu, wxPanel)
	EVT_BUTTON(singleCreateID_confirm, SingleGameCreateMenu::OnConfirm)
	EVT_BUTTON(singleCreateID_back, SingleGameCreateMenu::OnReturn)
wxEND_EVENT_TABLE()

SingleGameCreateMenu::SingleGameCreateMenu (wxWindow* p_parent)
	: wxPanel (p_parent), p_parent(p_parent) {

	title = new MyLabel(this, wxID_ANY, wxT("本地游戏"));
	game_select_label = new MyLabel(this, wxID_ANY, wxT("选择游戏："));
	game_select = new wxChoice(this, singleCreateID_game_select);
	user_name_label = new MyLabel(this, wxID_ANY, wxT("用户名："));
	user_name_input = new wxTextCtrl(this, singleCreateID_player_number, wxT("玩家 1"));
	user_number_label = new MyLabel(this, wxID_ANY, wxT("玩家数量："));
	user_number_input = new wxSpinCtrl(this, singleCreateID_player_number, wxT("4"));
	confirm = new MyButton(this, singleCreateID_confirm, wxT("确认"));
	go_back = new MyButton(this, singleCreateID_back, wxT("返回"));

	game_select->Insert(wxT("三人斗地主"), 0);
	game_select->Insert(wxT("四人斗地主"), 1);
	game_select->Insert(wxT("二对二"), 2);
	game_select->SetSelection(0);

	CenterBlockSizer *sizer = new CenterBlockSizer(this);

	sizer->AddWidget(title, true);
	sizer->AddWidget(game_select_label, true);
	sizer->AddWidget(game_select, false);
	sizer->AddWidget(user_name_label, true);
	sizer->AddWidget(user_name_input, false);
	sizer->AddWidget(user_number_label, true);
	sizer->AddWidget(user_number_input, false);
	sizer->AddWidget(confirm, true);
	sizer->AddWidget(go_back, false);

	sizer->Create();
	SetSizer(sizer);
}

void SingleGameCreateMenu::OnConfirm(wxCommandEvent &event) {
	get_controller(control)
	control->app_status.user_name = user_name_input->GetLineText(0);
	control->app_status.player_number = user_number_input->GetValue();
	switch (game_select->GetSelection()) {
		case 0:
			control->app_status.game_type = kLandlord3;
			break;
		case 1:
			control->app_status.game_type = kLandlord4;
			break;
		case 2:
			control->app_status.game_type = k2v2;
			break;
	}
	event.Skip();
}

void SingleGameCreateMenu::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(SingleGameJoinMenu, wxPanel)
	EVT_BUTTON(singleJoinID_confirm, SingleGameJoinMenu::OnConfirm)
	EVT_BUTTON(singleJoinID_back, SingleGameJoinMenu::OnReturn)
wxEND_EVENT_TABLE()

SingleGameJoinMenu::SingleGameJoinMenu(wxWindow *p_parent)
	: p_parent(p_parent), wxPanel(p_parent) {
	title = new MyLabel(this, wxID_ANY, wxT("加入本地游戏"));
	user_name_label = new MyLabel(this, wxID_ANY, wxT("用户名："));
	user_name_input = new wxTextCtrl(this, singleJoinID_name, wxT("玩家 1"));
	confirm = new MyButton(this, singleJoinID_confirm, wxT("确认"));
	go_back = new MyButton(this, singleJoinID_back, wxT("返回"));

	CenterBlockSizer * sizer = new CenterBlockSizer(this);
	sizer->AddWidget(title, true);
	sizer->AddWidget(user_name_label, true);
	sizer->AddWidget(user_name_input, false);
	sizer->AddWidget(confirm, true);
	sizer->AddWidget(go_back, false);
	sizer->Create();
	
	SetSizer(sizer);
}

void SingleGameJoinMenu::OnConfirm(wxCommandEvent &event) {
	get_controller(control);
	control->app_status.IP_address = string("127.0.0.1");
	control->app_status.user_name = user_name_input->GetLineText(0);
	event.Skip();
}

void SingleGameJoinMenu::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MultiGameMenu, wxPanel)
	EVT_BUTTON(multiID_join_game, MultiGameMenu::OnJoin)
	EVT_BUTTON(multiID_create_game, MultiGameMenu::OnCreate)
	EVT_BUTTON(multiID_back, MultiGameMenu::OnReturn)
wxEND_EVENT_TABLE()

MultiGameMenu::MultiGameMenu (wxWindow *p_parent)
	: wxPanel(p_parent), p_parent(p_parent) {
	
	title = new MyLabel(this, wxID_ANY, wxT("多人游戏"));
	join_game = new MyButton(this, multiID_join_game, wxT("加入游戏"));
	create_game = new MyButton(this, multiID_create_game, wxT("创建房间"));
	go_back = new MyButton(this, multiID_back, wxT("返回"));

	CenterBlockSizer *sizer = new CenterBlockSizer(this);

	sizer->AddWidget(title, true);
	sizer->AddWidget(join_game, true);
	sizer->AddWidget(create_game, true);
	sizer->AddWidget(go_back, true);
	sizer->Create();

	SetSizer(sizer);
}

void MultiGameMenu::OnJoin(wxCommandEvent &event) {
	get_controller(control);
	control->app_status.join_create = kJoin;
	event.Skip();
}

void MultiGameMenu::OnCreate(wxCommandEvent &event) {
	get_controller(control);
	control->app_status.join_create = kCreate;
	event.Skip();
}

void MultiGameMenu::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MultiGameJoinSetting, wxPanel)
	EVT_BUTTON(joinID_confirm, MultiGameJoinSetting::OnConfirm)
	EVT_BUTTON(joinID_back, MultiGameJoinSetting::OnReturn)
wxEND_EVENT_TABLE()

MultiGameJoinSetting::MultiGameJoinSetting (wxWindow *p_parent)
	: wxPanel(p_parent), p_parent(p_parent) {
	
	title = new MyLabel(this, wxID_ANY, wxT("加入游戏"));
	user_name_label = new MyLabel(this, wxID_ANY, wxT("用户名："));
	user_name_input = new wxTextCtrl(this, joinID_username, wxT("玩家 1"));
	IP_label = new MyLabel(this, wxID_ANY, wxT("房间 IP"));
	IP_input = new wxTextCtrl(this, joinID_IP);
	confirm = new MyButton(this, joinID_confirm, wxT("确认"));
	go_back = new MyButton(this, joinID_back, wxT("返回"));

	CenterBlockSizer *sizer = new CenterBlockSizer(this);

	sizer->AddWidget(title, true);
	sizer->AddWidget(user_name_label, true);
	sizer->AddWidget(user_name_input, false);
	sizer->AddWidget(IP_label, true);
	sizer->AddWidget(IP_input, false);
	sizer->AddWidget(confirm, true);
	sizer->AddWidget(go_back, false);
	sizer->Create();

	SetSizer(sizer);
}

void MultiGameJoinSetting::OnConfirm(wxCommandEvent &event) {
	get_controller(control);
	control->app_status.user_name = user_name_input->GetLineText(0);
	control->app_status.IP_address = IP_input->GetLineText(0);
	event.Skip();
}

void MultiGameJoinSetting::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(MultiGameCreateSetting, wxPanel)
	EVT_BUTTON(createID_confirm, MultiGameCreateSetting::OnConfirm)
	EVT_BUTTON(createID_back, MultiGameCreateSetting::OnReturn)
wxEND_EVENT_TABLE()

MultiGameCreateSetting::MultiGameCreateSetting(wxWindow *p_parent)
	: wxPanel(p_parent), p_parent(p_parent) {
	title = new MyLabel(this, wxID_ANY, wxT("创建游戏"));
	game_select_label = new MyLabel(this, wxID_ANY, wxT("选择游戏："));
	game_select = new wxChoice(this, createID_game_select);
	user_name_label = new MyLabel(this, wxID_ANY, wxT("用户名："));
	user_name_input = new wxTextCtrl(this, createID_username, wxT("玩家 1"));
	user_number_label = new MyLabel(this, wxID_ANY, wxT("玩家数量："));
	user_number_input = new wxSpinCtrl(this, createID_player_number, wxT("4"));
	confirm = new MyButton(this, createID_confirm, wxT("确认"));
	go_back = new MyButton(this, createID_back, wxT("返回"));

	game_select->Insert(wxT("三人斗地主"), 0);
	game_select->Insert(wxT("四人斗地主"), 1);
	game_select->Insert(wxT("二对二"), 2);
	game_select->SetSelection(0);

	CenterBlockSizer *sizer = new CenterBlockSizer(this);
	sizer->AddWidget(title, true, wxALIGN_CENTRE);
	sizer->AddWidget(game_select_label, true);
	sizer->AddWidget(game_select);
	sizer->AddWidget(user_name_label, true);
	sizer->AddWidget(user_name_input);
	sizer->AddWidget(user_number_label, true);
	sizer->AddWidget(user_number_input);
	sizer->AddWidget(confirm, true);
	sizer->AddWidget(go_back);
	sizer->Create();

	SetSizer(sizer);
}

void MultiGameCreateSetting::OnConfirm(wxCommandEvent &event) {
	get_controller(control);
	control->app_status.IP_address = "127.0.0.1";
	switch (game_select->GetSelection()) {
		case 0:
			control->app_status.game_type = kLandlord3;
			break;
		case 1:
			control->app_status.game_type = kLandlord4;
			break;
		case 2:
			control->app_status.game_type = k2v2;
			break;
	}
	control->app_status.user_name = user_name_input->GetLineText(0);
	control->app_status.player_number = user_number_input->GetValue();
	event.Skip();
}

void MultiGameCreateSetting::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

wxBEGIN_EVENT_TABLE(GamePending, wxPanel)
	EVT_TIMER(pendingID_timer, GamePending::OnTimer)
wxEND_EVENT_TABLE()

GamePending::GamePending(wxWindow *p_parent) : wxPanel(p_parent), p_parent(p_parent) {
	wait_label = new MyLabel(this, wxID_ANY, wxT("正在等待玩家加入，等待时长："));
	timer = new wxTimer(this, pendingID_timer);
	CenterBlockSizer *sizer = new CenterBlockSizer(this);
	sizer->AddWidget(wait_label, true);
	sizer->Create();
	SetSizer(sizer);
}

void GamePending::OnTimer(wxTimerEvent &event) {
	this->Layout();
	static char time_string[10];
	itoa(count, time_string, 10);
	switch (status) {
		case kWaiting:
			wait_label->SetLabelText(wxT("正在等待玩家加入，等待时长：") + wxString(time_string) + "s");
			break;
		case kJoining:
			wait_label->SetLabelText(wxT("正在加入房间，等待时长：") + wxString(time_string) + "s");
			break;
	}
	count++;
}

void GamePending::StartPending(Status set_status) {
	status = set_status;
	count = 0;
	timer->Start(1000);
}

void GamePending::StopPending() {
	count = 0;
	timer->Stop();
}

wxBEGIN_EVENT_TABLE(GameOver, wxPanel)
	EVT_BUTTON(overID_back, GameOver::OnReturn)
wxEND_EVENT_TABLE()

GameOver::GameOver(wxWindow *p_parent) : wxPanel(p_parent), p_parent(p_parent) {
	title = new MyLabel(this, wxID_ANY, wxT("游戏结束！"));
	score_label[0] = new MyLabel(this, wxID_ANY, "");
	score_label[1] = new MyLabel(this, wxID_ANY, "");
	score_label[2] = new MyLabel(this, wxID_ANY, "");
	score_label[3] = new MyLabel(this, wxID_ANY, "");
	go_back = new MyButton(this, overID_back, wxT("返回主菜单"));

	CenterBlockSizer *sizer = new CenterBlockSizer(this);
	sizer->AddWidget(title, true);
	sizer->AddWidget(score_label[0], true);
	sizer->AddWidget(score_label[1], true);
	sizer->AddWidget(score_label[2], true);
	sizer->AddWidget(score_label[3], true);
	sizer->AddWidget(go_back, true);
	sizer->Create();

	SetSizer(sizer);
}

void GameOver::Render() {
	for (int i = 0; i < 4; i++) {
		if (i < num_player) {
			wxString label_text;
			label_text << user_name[i] << ": " << score[i];
			score_label[i]->SetLabelText(label_text);
			score_label[i]->Show();
		} else {
			score_label[i]->Hide();
		}
	}
}

void GameOver::OnReturn(wxCommandEvent &event) {
	event.Skip();
}

void GameOver::SetScore(std::string user_name[], int score[], int num_player) {
	this->num_player = num_player;
	for (int i = 0; i < 4; i++) {
		this->user_name[i] = user_name[i];
		this->score[i] = score[i];
	}
}

wxBEGIN_EVENT_TABLE(GameInterface, wxPanel)
	EVT_BUTTON(gameID_deal, GameInterface::OnDeal)
	EVT_BUTTON(gameID_pass, GameInterface::OnPass)
	EVT_TIMER(gameID_count_down, GameInterface::OnTimer)
	EVT_BUTTON(gameID_auction0, GameInterface::OnBid)
	EVT_BUTTON(gameID_auction1, GameInterface::OnBid)
	EVT_BUTTON(gameID_auction2, GameInterface::OnBid)
	EVT_BUTTON(gameID_auction3, GameInterface::OnBid)
	EVT_COMMAND(eventID_refresh, RefreshEvent, GameInterface::OnRefresh)
	EVT_COMMAND(eventID_game_over, GameOverEvent, GameInterface::OnGameOver)
wxEND_EVENT_TABLE()

#include <thread>
#include <atomic>
#include <mutex>

// 用于交互
namespace GameStatus {
	std::mutex mtx;

	int num_players;			// 玩家的数量
	int num_cards[4];			// 牌的数量
	int landlord;				// 地主
	int stake;					// 倍数
	int count_down;				// 倒计时，开始倒计时的时候将其设置为倒计时的初始时间
	int smallest_bid;			// 最小初始分
	int largest_bid;			// 最大初始分

	bool is_counting_down;		// 是否正在倒计时
	bool show_stake;			// 是否显示倍数
	bool is_auction;			// 是否是叫分，叫分时候右下角显示四个按钮
	bool is_my_turn;			// 是否轮到我行动，轮到我行动的时候
	bool could_pass;			// 是否可以 pass
	bool show_info;				// 是否显示信息
	bool show_box;				// 显示弹窗
	bool is_denied;				// 上一轮出牌非法
	int  thinker;				// 谁正在思考

	string disp_info;			// 需要显示的信息
	CardSet my_cards;			// 我的牌
	CardSet last_round_card[4];	// 上一轮的牌
	int auction[4];				// 每个人叫的分数
	CardSet last_dealt;			// 上一轮我出的牌
	string user_name[4];		// 用户名
	string info;


	void dump () {
		cerr << "================= dump of status begin =================" << endl;
		cerr << "number of players: " << num_players << endl;
		cerr << "number of cards: " << num_cards[0] << " " << num_cards[1] << " "
			 << num_cards[2] << " " << num_cards[3] << endl;
		cerr << "landlord: " << landlord << endl;
		cerr << "stake: " << stake << endl;
		cerr << "count down: " << count_down << endl;
		cerr << "smallest bid: " << smallest_bid << endl;
		cerr << "largest bid: " << largest_bid << endl;

		cerr << "is counting down? " << boolalpha << is_counting_down << endl;
		cerr << "show stake? " << boolalpha << show_stake << endl;
		cerr << "is auction? " << boolalpha << is_auction << endl;
		cerr << "is my turn? " << boolalpha << is_my_turn << endl;
		cerr << "could pass? " << boolalpha << could_pass << endl;
		cerr << "show info? " << boolalpha << show_info << endl;
		cerr << "show box? "  << boolalpha << show_box << endl;
		cerr << "is denied? " << boolalpha << is_denied << endl;
		cerr << "thinker: " << thinker << endl;
		cerr << "User names: ";
		for (int i = 0; i < 4; i++)
			cerr << user_name[i] << " " << endl;
		cerr << "========================================================" << endl;
	}
	
	void ClearAuction() {
		for (int i = 0; i < 4; i++) {
			auction[i] = -1;
		}
	}

	void ClearAll() {
		is_counting_down = false;
		show_stake = false;
		is_auction = false;
		is_my_turn = false;
		could_pass = false;
		show_info = false;
		show_box = false;
		is_denied = false;
		thinker = -1;
	}

	void Restart() {
		ClearAll();
		ClearAuction();
		landlord = -1;
	}
}

GameInterface::GameInterface(wxWindow *p_parent)
	: wxPanel(p_parent), p_parent(p_parent), dc(this) {

	deck[0] = new DeckPanel(this, kFaceUp, kDown);
	deck[1] = new DeckPanel(this, kFaceDown, kRight);
	deck[2] = new DeckPanel(this, kFaceDown, kUp);
	deck[3] = new DeckPanel(this, kFaceDown, kLeft);

	user_info[0] = new MyLabel(this, wxID_ANY, wxT("玩家0"));
	user_info[1] = new MyLabel(this, wxID_ANY, wxT("玩家1"));
	user_info[2] = new MyLabel(this, wxID_ANY, wxT("玩家2"));
	user_info[3] = new MyLabel(this, wxID_ANY, wxT("玩家3"));

	midpan = new wxPanel(this);
	deal = new MyButton(this, gameID_deal, wxT("出牌"));
	pass = new MyButton(this, gameID_pass, wxT("跳过"));
	call_auction[0] = new MyButton(this, gameID_auction0, wxT("不叫"));
	call_auction[1] = new MyButton(this, gameID_auction1, wxT("叫1分"));
	call_auction[2] = new MyButton(this, gameID_auction2, wxT("叫2分"));
	call_auction[3] = new MyButton(this, gameID_auction3, wxT("叫3分"));
	timer_info = new MyLabel(midpan, wxID_ANY, wxT("回合剩余时间："));
	center_info = new MyLabel(midpan, wxID_ANY, wxT("测试信息："));
	stake_info = new MyLabel(midpan, wxID_ANY, wxT("倍率信息"));
	timer = new wxTimer(this, gameID_count_down);

	last_round[0] = new DeckPanel(midpan, kFaceUp, kCenter);
	last_round[1] = new DeckPanel(midpan, kFaceUp, kCenter);
	last_round[2] = new DeckPanel(midpan, kFaceUp, kCenter);
	last_round[3] = new DeckPanel(midpan, kFaceUp, kCenter);

	wxBoxSizer *mid_vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *mid_hbox = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *mid_hbox_inner = new wxBoxSizer(wxHORIZONTAL);

	mid_hbox->Add(last_round[3], 1, wxEXPAND);
	mid_hbox->Add(mid_vbox, 2, wxEXPAND);
	mid_hbox->Add(last_round[1], 1, wxEXPAND);

	mid_vbox->Add(last_round[2], 1, wxEXPAND);
	mid_vbox->Add(timer_info, 1, wxALIGN_CENTER);
	mid_vbox->Add(stake_info, 1, wxALIGN_CENTER);
	mid_vbox->Add(center_info, 1, wxALIGN_CENTER);
	mid_vbox->Add(last_round[0], 1, wxEXPAND);

	midpan->SetSizer(mid_hbox);

	wxBoxSizer *up_box = new wxBoxSizer(wxHORIZONTAL);
	up_box->Add(user_info[2], 2, wxALIGN_TOP);
	up_box->Add(deck[2], 8, wxEXPAND);

	wxBoxSizer *left_box = new wxBoxSizer(wxVERTICAL);
	left_box->Add(user_info[3], 2, wxALIGN_LEFT);
	left_box->Add(deck[3], 8, wxEXPAND);

	wxBoxSizer *right_box = new wxBoxSizer(wxVERTICAL);
	right_box->Add(user_info[1], 2, wxALIGN_RIGHT);
	right_box->Add(deck[1], 8, wxEXPAND);

	wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(left_box, 2, wxEXPAND);
	hbox->Add(midpan, 6, wxEXPAND);
	hbox->Add(right_box, 2, wxEXPAND);

	wxBoxSizer *down_box = new wxBoxSizer(wxHORIZONTAL);
	down_box->Add(user_info[0], 2, wxALIGN_TOP);
	down_box->Add(deck[0], 7, wxEXPAND);


	wxBoxSizer *button_box = new wxBoxSizer(wxVERTICAL);
	button_box->Add(deal, 1, wxALL, 5);
	button_box->Add(pass, 1, wxALL, 5);
	down_box->Add(button_box, 1, wxEXPAND);

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(up_box, 2, wxEXPAND);
	vbox->Add(hbox, 7, wxEXPAND);
	vbox->Add(down_box, 2, wxEXPAND);

	for (int i = 0; i < 4; i++) {
		button_box->Add(call_auction[i], 1, wxALL, 5);
	}
	deal->Hide();
	pass->Hide();
	for (int i = 0; i < 4; i++) {
		call_auction[i]->Hide();
	}

	SetSizer(vbox);
}

CardSet getCardSet(int n) {
	CardSet result(0);
	for (int i = 1; i <= n; i++) {
		result.Insert(1, 1);
	}
	return result;
}


#include "Client.h"
#include "message.h"

void JoinGameThread (AppStatus &status, Client& client, PageController *controller) {
	using namespace GameStatus;
	using GameConn::GameType;
	GameConn::GameType game_type;
	int my_pos;
	auto new_user_name = client.JoinRoom(status.IP_address, string(status.user_name), game_type, my_pos);
	if (new_user_name.size() == 0) { // 连接超时，异常
		wxCommandEvent *join_fail = new wxCommandEvent(JoinFailEvent, eventID_join_fail);
		controller->GetEventHandler()->QueueEvent(join_fail);
		return;
	}
	switch (game_type) {
		case GameType::Landlords_3:
			num_players = 3;
			break;
		case GameType::Landlords_4:
		case GameType::Doubleclasp:
			num_players = 4;
			break;
	}
	// 设置用户名
	for (int i = 0; i < num_players; i++) {
		user_name[i] = new_user_name[(i + my_pos) % num_players];
	}
	wxCommandEvent *join_success = new wxCommandEvent(JoinSuccessEvent, eventID_join_success);
	controller->GetEventHandler()->QueueEvent(join_success);
}

void CreateGameAndJoinThread (AppStatus& status, GameLauncher& launcher, Client& client, PageController* controller) {
	using namespace GameStatus;
	bool create_success = false;
	switch (status.game_type) {
		case kLandlord3:
			create_success = launcher.OpenRoom(GameConn::Landlords_3, status.player_number, 3 - status.player_number) == 0;
			break;
		case kLandlord4:
			create_success = launcher.OpenRoom(GameConn::Landlords_4, status.player_number, 4 - status.player_number) == 0;
			break;
		case k2v2:
			create_success = launcher.OpenRoom(GameConn::Doubleclasp, status.player_number, 4 - status.player_number) == 0;
			break;
	}
	if (create_success) {
		wxCommandEvent *success = new wxCommandEvent(CreateSuccessEvent, eventID_create_success);
		controller->GetEventHandler()->QueueEvent(success);
	} else {
		wxCommandEvent *fail = new wxCommandEvent(CreateFailEvent, eventID_create_fail);
		controller->GetEventHandler()->QueueEvent(fail);
	}
	
	// CPFA
	GameConn::GameType game_type;
	int my_pos;
	auto new_user_name = client.JoinRoom(status.IP_address, string(status.user_name), game_type, my_pos);
	if (new_user_name.size() == 0) { // 连接超时，异常
		wxCommandEvent *join_fail = new wxCommandEvent(JoinFailEvent, eventID_join_fail);
		controller->GetEventHandler()->QueueEvent(join_fail);
		return;
	}
	switch (game_type) {
		case GameType::Landlords_3:
			num_players = 3;
			break;
		case GameType::Landlords_4:
		case GameType::Doubleclasp:
			num_players = 4;
			break;
	}
	// 设置用户名
	for (int i = 0; i < num_players; i++) {
		user_name[i] = new_user_name[(i + my_pos) % num_players];
	}
	wxCommandEvent *join_success = new wxCommandEvent(JoinSuccessEvent, eventID_join_success);
	controller->GetEventHandler()->QueueEvent(join_success);
}

// 游戏结束时设置游戏结束时界面的参数
void GameOverSetting(Message message, GameInterface *game_interface) {
	using namespace GameStatus;
	my_cards = CardSet(0);
	for (int i = 0; i < 4; i++) {
		last_round_card[i] = CardSet(0);
		num_cards[i] = 0;
	}

	auto control = static_cast<PageController *>(game_interface->p_parent);
	int score[4];

	for (int i = 0; i < num_players; i++) {
		score[i] = message.GetInt(i);
	}

	control->game_over->SetScore(user_name, score, num_players);
}

inline int getPlayerId(int id, int num_players) {
	if (num_players == 4) {
		return id;
	}
	if (id < 2) {
		return id;
	}
	return 3;
}

void GameThread(Client &client, GameInterface *game_interface) {
	using namespace GameStatus;

	while (true) {
		bool end_loop = false;
		auto package = client.CollectGameMsg();
		mtx.lock();

		if (package.GetHeader().IsSuccess() == false) {	// 断开连接
			wxCommandEvent *log_out = new wxCommandEvent(LogOutEvent, eventID_log_out);
			game_interface->GetEventHandler()->QueueEvent(log_out);
			break;
		}
		auto message = Message(package.GetData());
		switch (message.GetType()) {
			case m_empty:
				ClearAll();
				break;
			case m_start:
				Restart();
				break;
			case m_end:
				ClearAll();
				end_loop = true;
				break;
			case m_box:
				ClearAll();
				show_info = true;
				disp_info = message.GetExtension();
				break;
			case m_disptext:
				ClearAll();
				show_box = true;
				disp_info = message.GetExtension();
				break;
			case m_dispeffect:
				break;
			case m_dispscore:
				ClearAll();
				GameOverSetting(message, game_interface);
				break;
			case m_think:
				ClearAll();
				thinker = message.GetPlayer();
				break;
			case m_deal:
				ClearAll();
				for (int i = 0; i < num_players; i++) {
					num_cards[i] = message.GetPar(i);
				}
				my_cards = message.GetCards();
				break;
			case m_bid:
				ClearAll();
				show_stake = true;
				if (message.IsRequest()) {
					is_auction = true;
					smallest_bid = message.GetPar(1);
					largest_bid = message.GetPar(2);
					count_down = message.GetTime();
				} else {
					is_auction = false;
					auction[message.GetPlayer()] = message.GetPar();
				}
				break;
			case m_changestake:
				ClearAll();
				show_stake = true;
				stake = message.GetPar();
				break;
			case m_setlandlord:
				ClearAll();
				landlord = message.GetPlayer();
				num_cards[message.GetPlayer()] = message.GetPar();
				last_round_card[message.GetPlayer()] = message.GetCards();
				if (message.GetPlayer() == 0) {	// 如果我叫了地主，需要合并手牌
					my_cards.Insert(message.GetCards());
				}
				break;
			case m_playout:
				ClearAll();
				ClearAuction();
				if (message.IsRequest()) { // 需要出牌
					is_counting_down = true;
					is_my_turn = true;
					count_down = message.GetTime();
					could_pass = message.GetPar();
				} else {
					num_cards[message.GetPlayer()] = message.GetPar();
					last_round_card[message.GetPlayer()] = message.GetCards();
				}
				break;
			case m_deny:
				is_denied = true;
				break;
			default:
				break;
		}
		mtx.unlock();
		if (end_loop) {
			break;
		}
		wxCommandEvent *refresh = new wxCommandEvent(RefreshEvent, eventID_refresh);
		game_interface->GetEventHandler()->QueueEvent(refresh);
	}
	wxCommandEvent *over = new wxCommandEvent(GameOverEvent, eventID_game_over);
	game_interface->GetEventHandler()->QueueEvent(over);
}

void GameInterface::StartGame(Client &client) {
	center_info->SetLabelText(wxT("游戏进行中"));
	center_info->Show();
	timer->Start(1000);

	std::thread game_thread(GameThread, std::ref(client), this);
	game_thread.detach();
}

// 根据当前的状态来渲染房间
void GameInterface::Render() {
	using namespace GameStatus;
	mtx.lock();

	if (is_denied) {
		wxMessageBox(wxT("出牌不合法！"));
		my_cards.Insert(last_dealt);
	}
	
	for (int i = 0; i < 4; i++) {
		last_round[i]->SetThinking(false);
	}
	
	if (thinker != -1) {
		last_round[getPlayerId(thinker, num_players)]->SetThinking(true);
	}

	for (int i = 0; i < num_players; i++) {
		last_round[getPlayerId(i, num_players)]->SetBidding(auction[i]);
	}

	deck[0]->SetDeck(my_cards);
	last_round[0]->SetDeck(last_round_card[0]);
	user_info[0]->SetLabelText(user_name[0]);
	user_info[0]->Show();

	deck[1]->SetDeck(getCardSet(num_cards[1]));
	last_round[1]->SetDeck(last_round_card[1]);
	user_info[1]->SetLabelText(user_name[1]);
	user_info[1]->Show();

	if (num_players == 3) {
		deck[2]->Hide();
		last_round[2]->Hide();
		user_info[2]->Hide();

		deck[3]->SetDeck(getCardSet(num_cards[2]));
		last_round[3]->SetDeck(last_round_card[2]);
		user_info[3]->SetLabelText(user_name[2]);
		user_info[3]->Show();
	} else if (num_players == 4) {
		deck[2]->SetDeck(getCardSet(num_cards[2]));
		deck[2]->Show();
		last_round[2]->SetDeck(last_round_card[2]);
		last_round[2]->Show();
		user_info[2]->SetLabelText(user_name[2]);
		user_info[2]->Show();

		deck[3]->SetDeck(getCardSet(num_cards[3]));
		last_round[3]->SetDeck(last_round_card[3]);
		user_info[3]->SetLabelText(user_name[3]);
		user_info[3]->Show();
	}

	if(landlord != -1) {
		user_info[getPlayerId(landlord, num_players)]->SetLabelText(user_name[landlord] + wxT("（地主）"));
	}

	this->Layout();

	wxString stake_info_string("");
	stake_info_string << wxT("当前倍数：") << stake;
	stake_info->SetLabelText(stake_info_string);

	center_info->SetLabelText(disp_info);

	if(is_counting_down) {
		timer_info->Show();
	} else {
		timer_info->Hide();
	}

	if (show_stake) {
		stake_info->Show();
	} else {
		stake_info->Hide();
	}

	if (show_info) {
		center_info->Show();
	} else {
		center_info->Hide();
	}

	if (show_box) {
		wxMessageBox(disp_info);
	}

	if (is_my_turn) {
		deal->Show();
	} else {
		deal->Hide();
	}
	
	if (could_pass) {
		pass->Show();
	} else {
		pass->Hide();
	}
	
	for (int i = 0; i < 4; i++) {
		call_auction[i]->Hide();
	}

	if (is_auction) {
		call_auction[0]->Show();
		for (int i = smallest_bid; i <= largest_bid; i++) {
			call_auction[i]->Show();
		}
	}

	this->Layout();

	for (int i = 0; i < 4; i++) {
		deck[i]->Render();
		last_round[i]->Render();
	}
	mtx.unlock();
}

void GameInterface::OnRefresh(wxCommandEvent &event) {
	Render();
}

void GameInterface::OnLogOut(wxCommandEvent &event) {
	wxMessageBox(wxT("连接已断开！"));
}

void GameInterface::OnDeal(wxCommandEvent &event) {
	using namespace GameStatus;
	mtx.lock();
	last_dealt = deck[0]->GetDrawnDeck();
	deck[0]->Release();
	my_cards.Delete(last_dealt);
	mtx.unlock();

	get_controller(control);
	auto client = control->GetClient();
	Message new_message;
	new_message.SetCards(last_dealt);

	Package new_package(Header(1, i_server), new_message.String());
	client->SendGameMsg(new_package);

}

void GameInterface::OnBid(wxCommandEvent &event) {
	int my_bid = 0;
	switch(event.GetId()) {
		case gameID_auction0:
			my_bid = 0;
			break;
		case gameID_auction1:
			my_bid = 1;
			break;
		case gameID_auction2:
			my_bid = 2;
			break;
		case gameID_auction3:
			my_bid = 3;
			break;
	}
	get_controller(control);
	auto client = control->GetClient();
	Message new_message;
	new_message.SetPar(0, my_bid);

	Package new_package(Header(1, 0), new_message.String());
	client->SendGameMsg(new_package);
}

void GameInterface::OnPass(wxCommandEvent &event) {
	get_controller(control);
	Message new_message;
	new_message.SetCards(CardSet(0));
	Package new_package(Header(1, 0), new_message.String());
	control->client.SendGameMsg(new_package);
}

void GameInterface::OnTimer(wxTimerEvent &event) {
	using namespace GameStatus;
	if (!is_counting_down) {
		return;
	}
	if (count_down <= 0) {	// 倒计时时间到了
		// 停止倒计时
		is_counting_down = false;

		// 发送空卡组代表不出牌
		CardSet null_set;
		Message new_message;
		new_message.SetCards(null_set);
		Package new_package(Header(1, 0), new_message.String());
		get_controller(control);
		auto client = control->GetClient();
		client->SendGameMsg(new_package);
		return;
	}
	count_down--;
	wxString timer_info_string("");
	timer_info_string << wxT("回合剩余时间：") << count_down << "s";
	timer_info->SetLabel(timer_info_string);
}

void GameInterface::OnGameOver(wxCommandEvent &event) {
	event.Skip();
}