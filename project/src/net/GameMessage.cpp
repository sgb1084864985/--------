// GameMessage.cpp
//
#include "GameMessage.h"

int GameMessage::to_str(char* inout)const{
	char* p;
	string out;
	int len = extra_info.size();
	int state = this->state;

	p = reinterpret_cast<char*>(&state);
	out.append(p,sizeof(state));

	p = reinterpret_cast<char*>(&len);
	out.append(p, sizeof(len));
	if (len) {
		out.append(extra_info);
	}
	memcpy(inout, out.c_str(), out.size());
	return out.size();
}

void GameMessage::setGameType(GameType t) {
	char* p = reinterpret_cast<char*>(&t);
	setGameType(p);
}

void GameMessage::setGameType(char*p) {
	state = ConnMsg::MSG_GAME_TYPE;
	extra_info.assign(p, sizeof(GameType));
}

GameType GameMessage::getGameType()const {
	char buf[BUF_SIZE];
	GameType* g = reinterpret_cast<GameType*>(buf);
	memcpy(buf, extra_info.c_str(), sizeof(GameType));
	return *g;
}

void GameMessage::setInt(int integer) {
	char* p = reinterpret_cast<char*>(&integer);
	setInt(p);
}

void GameMessage::setInt(char* p) {
	state = ConnMsg::MSG_SEND_INT;
	extra_info.assign(p, sizeof(int));
}

int GameMessage::getInt() const{
	int integer;
	memcpy(&integer, extra_info.c_str(), sizeof(int));
	return integer;
}

void GameMessage::setPackage(const Package& pkg) {
	state = ConnMsg::MSG_PAKAGE;
	extra_info = pkg.GetData();
}

void GameMessage::addName(const string& name) {
	state = ConnMsg::MSG_SEND_NAME;
	extra_info = name;
}

int GameMessage::read_head(const char*src) {
	int len;
	memcpy(&state, src, sizeof(state));
	memcpy(&len, src+sizeof(state), sizeof(len));
	return len;
}

void GameMessage::setSignal(msg_t ms) {
	state = ms;
	extra_info.clear();
}

Package GameMessage::getPackage() {
	if (state != ConnMsg::MSG_PAKAGE) {
		//error_handle;
	}
	Header head(true, 0);
	Package pkg(head,extra_info);
	return pkg;
}

Package GameMessage::getPackage(char* src, int len) {
	Header head(true, 0);
	string info(src, len);
	Package pkg(head, info);
	return pkg;
}


//
//inline GameMessage:: GameMessage(){}
//
//inline GameMessage:: GameMessage(CONNECT_STATE state,int card_owner_index=-1,const CardSet* LPcards=nullptr){
//    SetGameMessage(state,card_owner_index,LPcards);
//}
//
//inline GameMessage:: ~GameMessage(){}
//
//inline void GameMessage::clear(){
//    cards.clear();
//}
//
//inline void GameMessage::SetGameMessage(CONNECT_STATE state,int card_owner_index,const CardSet* LPcards){
//    this->state=state;
//    if(LPcards) cards=*LPcards;
//    if(card_owner_index!=-1) this->card_owner_index=card_owner_index;
//}
//
//inline int GameMessage::get_card_owner_index(){
//    return card_owner_index;
//}
//
//inline CONNECT_STATE GameMessage::get_state(){
//    return state;
//}
//
//inline const CardSet& GameMessage::get_cards(){
//    return cards;
//}
//
//int GameMessage::cards_buf2msg(const void* buf,int n){
//    const int* array=(int*)(buf);
//    int i=0;
//    cards.clear();
//    for(i=0;i<n;i++) cards.push_back(array[i]);
//    return 0;
//}
//
//int GameMessage::msg2cards_buf(void* buf,int max_n)const{
//    if(max_n<1) return;
//    int len=cards.size();
//    int i,j;
//    unsigned char*p;
//    int block;
//    int blocksize=sizeof(block);
//
//    memcpy(buf,&len,blocksize);
//    p=(unsigned char*)buf+blocksize;
//
//    for(i=0;i<len && i<max_n-1;i++){
//        block=cards.at(i);
//        memcpy(p,&block,blocksize);
//        p+=blocksize;
//    }
//    return min(len+1,max_n);
//}
//
//int GameMessage::msg2buf(void* buf,int max_size)const{
//    if(max_size<2*sizeof(int)) return 0;
//    int size;
//    memcpy(buf,&state,sizeof(CONNECT_STATE));
//    size+=sizeof(CONNECT_STATE);
//    if(state==GAME_SEND_YOU_CARD||state==GAME_GIVE_ME_CARD){
//        memcpy((char*)buf+size,&card_owner_index,sizeof(int));
//        size+=sizeof(int);
//        if(state==GAME_SEND_YOU_CARD){
//            size+=sizeof(Card)*
//                msg2cards_buf(
//                (char*)buf+size,(max_size-size)/sizeof(Card));
//        }
//    }
//    return size;
//}