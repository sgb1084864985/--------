// ConnectBase.h
//g++ -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8
#ifndef _CONNECT_BASE_H
#define _CONNECT_BASE_H
// #include "game.h"
//#define WIN32_LENA_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ConnException.h"
#include <WinSock2.h>
#include <process.h>
#include <iostream>
#include <vector>
#include <array>

#include "package.h"

using std::string;
using std::array;
#define i_server (-1)

namespace GameConn {
    enum GameType {
        Landlords_3 = 0,
        Landlords_4,
        Doubleclasp=127,
    };
}

using GameConn::GameType;

class ConnectBase{
     private:
     protected:
         static const int BUF_SIZE=1024;
     public:
         //static void ErrorHandling(const char* message);
         //static int read_msg(SOCKET s,GameMessage& msg);
         //static char* read_buf(SOCKET s,void* dst,char* buf,char* cur_read,int buf_size,int max_size);
};

#endif