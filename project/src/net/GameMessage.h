// GameMessage.h
#ifndef _GAME_MESSAGE_H
#define _GAME_MESSAGE_H
#include "ConnectBase.h"
#include "package.h"

//using namespace game_connect;
namespace ConnMsg {
    enum msg_t {
        MSG_PAKAGE=0,
        MSG_CONN_DENY,
        MSG_CONN_ACCEPT,
        MSG_WANT_CLOSE,
        MSG_GRANT_CLOSE,
        MSG_SEND_NAME,
        MSG_RECV_NAME,
        MSG_SEND_INT,
        MSG_GAME_TYPE=20,
    };
}

using ConnMsg::msg_t;

class GameMessage:public ConnectBase {
    private:
        msg_t state;
        string extra_info;
        //int card_owner_index;
        //CardSet cards;
    public:
        int head_size() { return sizeof(state) + sizeof(int); }
        int read_head(const char*);
        int to_str(char*)const;
        void setPackage(const Package& pkg);
        Package getPackage();
        static Package getPackage(char* src,int len);
        const string& getName() { return extra_info; }
        static string getName(char* src, int len) { return string(src, len); }
        void addName(const string& name);
        void setSignal(msg_t ms);
        void setGameType(GameType t);
        void setGameType(char* p);
        void setInt(int integer);
        void setInt(char* p);

        GameMessage():state(ConnMsg::MSG_PAKAGE){}
        msg_t get_state() { return state; }

        void readPackage(char* src, int len) {
            setPackage(getPackage(src, len));
        }
        GameType getGameType()const;
        int getInt()const;
        //int get_card_owner_index();

        //void clear();

        //GameMessage(CONNECT_STATE state,int card_owner_index=-1,const CardSet* LPcards=nullptr);

        //const CardSet& get_cards();
        //void SetGameMessage(CONNECT_STATE state,int card_owner_index=-1,const CardSet* LPcards=nullptr);

        //int cards_buf2msg(const void* buf,int n);
        //int msg2cards_buf(void* buf,int max_n)const;//max_n: the block size of the buf,including the space of length;return block(a card) size
        //int msg2buf(void* buf,int max_size)const;//return size(bytes)
};
#endif

