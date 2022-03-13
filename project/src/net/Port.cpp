// Port.cpp

#include "Port.h"

Port::Port(PortState state,USHORT port):error(0),state(state){
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
}

void Port::SendSignal(msg_t sig, SOCKET to) {
    GameMessage gm;
    gm.setSignal(sig);
    SendInfo(gm, to);
}

void Port::SendGameType(GameType gt, SOCKET to) {
    GameMessage gm;
    gm.setGameType(gt);
    SendInfo(gm, to);
}

void Port::SendInt(int integer, SOCKET to) {
    GameMessage gm;
    gm.setInt(integer);
    SendInfo(gm, to);
}

void Port::SendInfo(const GameMessage& gm, SOCKET clntsocket){
    if(clntsocket==INVALID_SOCKET||clntsocket==0){
        //return MESSAGE_CANCELED;
        throw ceNullSocket();
    }
    char buf[BUF_SIZE];
    int len;
    len = gm.to_str(buf);
    send(clntsocket, buf, len, 0);
}

void Port::read_buf(SOCKET s, char* dst, int size) {
    int len = 0;
    int recv_len;
    while (len < size) {
        recv_len = recv(s, dst, size - len, 0);
        if (recv_len == 0||recv_len==-1) {
            throw ceReadInt();
        }
        len += recv_len;
    }
}

void Port::SendName(const string& name, SOCKET to) {
    GameMessage gm;
    gm.addName(name);
    SendInfo(gm, to);
}

msg_t Port::read_msg(SOCKET s,GameMessage& gm,Package& pkg) {
    char buf[BUF_SIZE];
    int len;

    read_buf(s, buf, gm.head_size());
    len=gm.read_head(buf);
    if (len) {
        read_buf(s, buf, len);
        switch (gm.get_state()) {
        case ConnMsg::MSG_PAKAGE:
            pkg = gm.getPackage(buf, len);
            break;
        case ConnMsg::MSG_SEND_NAME:
            names.push_back(gm.getName(buf, len));
            break;
        case ConnMsg::MSG_GAME_TYPE:
            gm.setGameType(buf);
            break;
        case ConnMsg::MSG_SEND_INT:
            gm.setInt(buf);
            break;
        default:break;
        }
    }
    return gm.get_state();
}
//int Port::MsgBufClear(queue<GameMessage>& buf){
//
//}

//char* Port::read_buf(SOCKET s,
//    void* dst,
//    char* buf, 
//    char* cur_read, 
//    int buf_size, 
//    int max_size
//) {
//    int strLen;
//    char* p = cur_read;
//    while (p - buf < buf_size) {
//        strLen = recv(s, p, max_size - (p - buf), 0);
//        p += strLen;
//    }
//    if (dst)memcpy(dst, buf, buf_size);
//    return p;
//}

//CONNECT_STATE Port::SendInfo(const GameMessage& msg, SOCKET clntsocket)const{

//    char buf[BUF_SIZE];
//    int len;
//    len=msg.msg2buf(buf,BUF_SIZE);
//    send(clntsocket,buf,len,0);
//    return MESSAGE_SENT;
//}

//void Port::SendGameMsg(const Package& p) const {
//    GameMessage gm;
//    gm.setPackage(p);
//    SendInfo(gm,)
//}