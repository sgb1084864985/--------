//Server.h
#ifndef _SERVER_H
#define _SERVER_H
// #include "GameMessage.h"
#include "Port.h"
//#include "game.h"
#include <queue>

// class Package;
// using namespace game_connect;

class Server:public Port{

public:
    /* for game.h */
    void SendGameMsg(const Package &p);
    Package CollectGameMsg(int sender=i_server);
    const string& GetClientNameR(int k) const;

    /* for self */
    int OpenRoom(GameType gt,int humans,int robots);  
    //int StartGame();

    Server();
    ~Server();

    GameType getGameType()const{ return game_t; }
    int getHumans()const{ return humans; }
    int getRobots()const{ return robots; }
    PortState getState()const { return state; }
    void setState(PortState state) { this->state = state; }
    int isError()const { return error; }
    int isReady()const { return ready; }
private:
    //...
    int humans;
    int robots;
    int ready;
    //HANDLE gameLock

    struct index_info {
        Server* s;
        int index;
    };


    static const int MAX_PLAYERS = 4;
    static const int MSG_BUF_SIZE = 10;

    int num_Members;
    HANDLE Mutex[MAX_PLAYERS];
    queue<Package> buffer[MAX_PLAYERS+1];

    SOCKET Member_Sock[MAX_PLAYERS + 1];

    int num_Players;
    int num_left;
    int PlayerSortedSock[MAX_PLAYERS];

    WSAEVENT Member_Event[MAX_PLAYERS + 1];
    //queue<GameMessage> Buf[MAX_PLAYERS + 1];

    int handle_accept();
    int handle_read(int index);
    int handle_close(int index);
    int BufClear(int index);
    int CloseRoom();
    SOCKET getSock(int i)const{ 
        if (PlayerSortedSock[i]==0) {
            throw ceNullSocket();
        }
        return Member_Sock[PlayerSortedSock[i]]; 
    }
    //void setGame();
    // CONNECT_STATE SendGameInfo(const GameMessage& msg, SOCKET clntsocket)const;

    static unsigned WINAPI thread_server_main(void* LPserver);
    static unsigned WINAPI thread_server_BufNotEmpty(void* arg);
};
#endif