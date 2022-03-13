#include "Client.h"
//Client.cpp

Client::Client():serv_Sock(0),Port(){

}

const vector<string>& Client::JoinRoom(
    const char* IP,
    const string& name,
    GameType& gameType,
    int& pos
){
    if (state != GamePort::GAME_OVER) return names;
	WSADATA wsaData;

    GameMessage gm;
    Package pkg(Header(0, 0),"");
    int loop_flag = true;

    try {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw ceNetEx("WSAStartup() error!");
        serv_Sock = socket(PF_INET, SOCK_STREAM, 0);
        if (serv_Sock == INVALID_SOCKET)
            throw ceNetEx("socket() error!");

        addr.sin_addr.s_addr = inet_addr(IP);

        if (connect(serv_Sock, (SOCKADDR*)&addr, 
            sizeof(addr)) == SOCKET_ERROR)
            throw ceNetEx("connect() error!");

        SendName(name, serv_Sock);
        while (loop_flag) {
            switch (read_msg(serv_Sock, gm, pkg)) {
            case ConnMsg::MSG_CONN_ACCEPT:
                loop_flag = false;
                state = GamePort::GAME_OPEN;
                break;
            case ConnMsg::MSG_CONN_DENY:
                loop_flag = false;
                closesocket(serv_Sock);
                break;
            case ConnMsg::MSG_GAME_TYPE:
                gameType = gm.getGameType();
                break;
            case ConnMsg::MSG_SEND_INT:
                pos = gm.getInt();
                break;
            default: break;
            }
        }
    }
    catch (ceNetEx& e) {
        names.clear();
    }
    //if (names.size() == 3) {
    //    names.push_back(names[2]);
    //}
    return names;
}

int Client::QuitRoom() {
    if (serv_Sock) {
        SendSignal(ConnMsg::MSG_WANT_CLOSE, serv_Sock);
        GameMessage gm;
        Package pkg(Header(0, 0),"");
        try {
            while (read_msg(serv_Sock, gm, pkg)
                != ConnMsg::MSG_GRANT_CLOSE);
        }
        catch (...) {

        }
        state = GamePort::GAME_OVER;
        closesocket(serv_Sock);
        WSACleanup();
        return 0;
    }
}

void Client::SendGameMsg(const Package & p){
    if (error) return;
    GameMessage gm;
    try {
        gm.setPackage(p);
        SendInfo(gm, serv_Sock);
    }
    catch (logic_error& e) {
        cout << e.what();
        error = 1;
    }
}

Package Client::CollectGameMsg(int sender){
    GameMessage gm;
    Package pkg(Header(0,0),"");
    if (!error) {
        try {
            while (read_msg(serv_Sock, gm, pkg)
                != ConnMsg::MSG_PAKAGE);
        }
        catch (ceReadInt& e) {
            pkg = Package(Header(0, 0),"");
            error = 1;
        }
    }
    return pkg;
}

//inline int Client::SendPlayerInfo(const GameMessage& msg){
//    SendInfo(msg,serv_Sock);
//}

