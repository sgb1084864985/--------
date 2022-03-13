// ConnectBase.cpp

#include "ConnectBase.h"

//int ConnectBase::read_msg(SOCKET s,GameMessage& msg){
//    //buf structure: |state|index|num_cards|card1|card2|...|
//    int index=-1;
//    int num_cards;
//    char buf[BUF_SIZE];
//    CONNECT_STATE state;
//    int blocksize=sizeof(int);
//    int strLen=0;
//    char* p=buf;
//
//    p=read_buf(s,&state,buf,p,blocksize,BUF_SIZE);
//    if(state==GAME_SEND_YOU_CARD||state==GAME_GIVE_ME_CARD){
//        p=read_buf(s,&index,buf+blocksize,p,blocksize,BUF_SIZE);
//        if(state==GAME_SEND_YOU_CARD){
//            p=read_buf(s,&num_cards,buf+2*blocksize,p,blocksize,BUF_SIZE);
//            read_buf(s,nullptr,buf+3*blocksize,p,blocksize*num_cards,BUF_SIZE);
//            msg.cards_buf2msg(buf+3*blocksize,num_cards);
//        }
//    }
//    msg.SetGameMessage(state,index);
//    return 0;
//}
