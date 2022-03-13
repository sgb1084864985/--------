#ifndef PAGE_CONTROL_EVENT_H
#define PAGE_CONTROL_EVENT_H
enum {
	mainID_play_single = 1000,
	mainID_play_multi
};

enum {
	singleID_create = 1500,
	singleID_join,
	singleID_back
};

enum {
	singleCreateID_game_select = 2000,
	singleCreateID_username,
	singleCreateID_player_number,
	singleCreateID_confirm,
	singleCreateID_back
};

enum {
	singleJoinID_confirm = 2500,
	singleJoinID_back,
	singleJoinID_name
};

enum {
	multiID_join_game = 3000,
	multiID_create_game,
	multiID_back
};

enum {
	joinID_username = 4000,
	joinID_IP,
	joinID_passwd,
	joinID_confirm,
	joinID_back
};

enum {
	createID_game_select = 5000,
	createID_username,
	createID_player_number,
	createID_passwd,
	createID_confirm,
	createID_back
};

enum {
	gameID_deal = 6000,
	gameID_pass,
	gameID_count_down,
	gameID_auction0,
	gameID_auction1,
	gameID_auction2,
	gameID_auction3
};

enum {
	pendingID_timer = 7000
};

enum {
	overID_back = 8000
};

// 同步并行代码使用的事件 id
enum {
	eventID_join_success = 9000,
	eventID_join_fail,
	eventID_create_success,
	eventID_create_fail,
	eventID_refresh,
	eventID_game_over,
	eventID_log_out,
	eventID_denied,
	eventID_disp_box,
	eventID_disp_text
};

#endif