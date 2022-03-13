// Client.h
#ifndef _CLIENT_H
#define _CLIENT_H
#include "ConnectBase.h"
#include "Port.h"
//using namespace game_connect;

// #define i_server -1 //这个在头文件里
// class Package;

class Client:public Port{
private:
        SOCKET serv_Sock;
        //queue<GameMessage> Buf;
public:

    Client();
    const vector<string>& JoinRoom(const char* IP,const string& name,GameType& gameType,int&pos); // 接收或超时后才返回
    int QuitRoom();

    void SendGameMsg(const Package &p);
    Package CollectGameMsg(int sender=i_server);

    // int FetchGameInfo(GameMessage& msg); //立即返回，若服务器有信息传回返回message_fetched，否则返回no_message
    // int SendPlayerInfo(const GameMessage& msg);
};

#endif