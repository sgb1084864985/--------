// Port.h
#ifndef _PORT_H
#define _PORT_H

#include "ConnectBase.h"
#include "GameMessage.h"
#include <queue>
// using namespace game_connect;

namespace GamePort {
    enum PortState {
        GAME_ON = 0,
        GAME_OVER,
        GAME_OPEN = 127,
    };
}

using GamePort::PortState;

class Port:public ConnectBase{
    protected:
        PortState state;
        SOCKADDR_IN addr;
        vector<string>names;
        GameType game_t;
        int error;

        msg_t read_msg(SOCKET s, GameMessage& gm, Package& pkg);

        static void SendSignal(msg_t sig, SOCKET to);
        static void SendName(const string& name, SOCKET to);
        static void SendGameType(GameType gt, SOCKET to);
        static void SendInt(int integer, SOCKET to);
        //static char* read_buf(SOCKET s,void* dst,char* buf,char* cur_read,int buf_size,int max_size);
        static void read_buf(SOCKET s, char* dst, int size);
        static int MsgBufClear(queue<GameMessage>& buf);
        static void SendInfo(const GameMessage& gm, SOCKET clntsocket);

    public:
        Port(PortState state = GamePort::GAME_OVER, USHORT port = 9190);
        virtual void SendGameMsg(const Package& p) = 0;
        virtual Package CollectGameMsg(int sender=i_server)=0;
};
#endif