#pragma once
#include "wx/event.h"

wxDECLARE_EVENT(JoinSuccessEvent, wxCommandEvent);		// 加入房间成功
wxDECLARE_EVENT(JoinFailEvent, wxCommandEvent);			// 加入房间失败
wxDECLARE_EVENT(CreateSuccessEvent, wxCommandEvent);	// 创建房间成功
wxDECLARE_EVENT(CreateFailEvent, wxCommandEvent);		// 创建房间失败
wxDECLARE_EVENT(RefreshEvent, wxCommandEvent);			// 刷新游戏界面
wxDECLARE_EVENT(LogOutEvent, wxCommandEvent);			// 断开连接
wxDECLARE_EVENT(DeniedEvent, wxCommandEvent);			// 出牌被拒绝
wxDECLARE_EVENT(ChangeStatus, wxCommandEvent);			// 游戏状态改变
wxDECLARE_EVENT(GameOverEvent, wxCommandEvent);			// 游戏结束