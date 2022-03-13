#include "events.h"

wxDEFINE_EVENT(JoinSuccessEvent, wxCommandEvent);		// 加入房间成功
wxDEFINE_EVENT(JoinFailEvent, wxCommandEvent);			// 加入房间失败
wxDEFINE_EVENT(CreateSuccessEvent, wxCommandEvent);		// 创建房间成功
wxDEFINE_EVENT(CreateFailEvent, wxCommandEvent);		// 创建房间失败
wxDEFINE_EVENT(RefreshEvent, wxCommandEvent);			// 刷新游戏界面
wxDEFINE_EVENT(LogOutEvent, wxCommandEvent);			// 断开连接
wxDEFINE_EVENT(DeniedEvent, wxCommandEvent);			// 出牌被拒绝
wxDEFINE_EVENT(ChangeStatus, wxCommandEvent);			// 游戏状态改变
wxDEFINE_EVENT(GameOverEvent, wxCommandEvent);			// 游戏结束