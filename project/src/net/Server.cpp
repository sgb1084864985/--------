// Server.cpp

#include "Server.h"
//#include "game.h"

void Server::SendGameMsg(const Package& p) {
    if (error) return;
    try {
        int i = p.GetHeader().GetRecipient();
        GameMessage gm;
        gm.setPackage(p);
        SendInfo(gm, getSock(i));
    }
    catch (logic_error& e) {
        // error print
        cout << "failed to send massage" << endl;
        cout << e.what();
        error = 1;
    }
}

const string& Server::GetClientNameR(int k) const {
    static string tmp;
    if (k >= names.size() || k < 0) return tmp;
    return names[k];
}

Package Server::CollectGameMsg(int sender) {
    HANDLE hThread;
    unsigned threadID;
    index_info Info;
    Package pkg(Header(0, i_server),"");

    int microsecond = INFINITE;
    int index = PlayerSortedSock[sender];
    if (index == 0||error) return pkg;

    try {
        Info.s = this;
        Info.index = index;

        hThread = (HANDLE)_beginthreadex(
            NULL,
            0,
            thread_server_BufNotEmpty,
            (void*)&Info,
            0,
            &threadID
        );
        WaitForSingleObject(hThread, (DWORD)microsecond);
        if (!buffer[index].empty()) {
            WaitForSingleObject(Mutex[index - 1], INFINITE);
            pkg = buffer[index].front();
            buffer[index].pop();
            ReleaseMutex(Mutex[index - 1]);
        }
    }
    catch (logic_error& e) {
        // error print
        cout << "failed to read massage" << endl;
        pkg = Package(Header(0, 0),"");
    }
    return pkg;
}

unsigned WINAPI Server::thread_server_BufNotEmpty(void* arg){
    index_info* pInfo=reinterpret_cast<index_info*>(arg);
    Server* s=pInfo->s;
    int index=pInfo->index;
    try {
        if (s) {
            while (s->buffer[index].empty());
        }
        else throw ceNullServer();
    }
    catch (logic_error&e) {
        cout << e.what();
        if(s)s->error = 1;
    }
}

unsigned WINAPI Server::thread_server_main(void* LPserver){
    Server* s = reinterpret_cast<Server*>(LPserver);
    WSANETWORKEVENTS netEvents;

    int posInfo,startIdx;
    int sigEventIdx;
    int i;


    try {
        if (s == nullptr) {
            throw ceNullServer();
        }
        else {
            //WaitForSingleObject(s->gameLock, INFINITE);

            posInfo =
                WSAWaitForMultipleEvents(
                    s->num_Members,
                    s->Member_Event,
                    FALSE, WSA_INFINITE, FALSE
                );

            startIdx = posInfo - WSA_WAIT_EVENT_0;

            while (s->state != GamePort::GAME_OVER) {
                for (i = startIdx; i < s->num_Members; i++) {
                    sigEventIdx =
                        WSAWaitForMultipleEvents(
                            1, &s->Member_Event[i],
                            TRUE, 0, FALSE
                        );

                    if ((sigEventIdx == WSA_WAIT_FAILED ||
                        sigEventIdx == WSA_WAIT_TIMEOUT)) {
                        continue;
                    }
                    else {
                        sigEventIdx = i;
                        WSAEnumNetworkEvents(
                            s->Member_Sock[sigEventIdx],
                            s->Member_Event[sigEventIdx],
                            &netEvents
                        );
                        /* 请求连接时 */
                        if (netEvents.lNetworkEvents & FD_ACCEPT) {
                            if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
                                throw ceIOError("Accept Error");
                                break;
                            }
                            s->handle_accept();
                        }

                        if (netEvents.lNetworkEvents & FD_READ) {
                            if (netEvents.iErrorCode[FD_READ_BIT] != 0) {
                                throw ceIOError("Read Error");
                                break;
                            }
                            s->handle_read(sigEventIdx);
                        }

                        /* 断开连接 */
                        if (netEvents.lNetworkEvents & FD_CLOSE) {
                            if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
                                throw ceIOError("Close Error");
                                break;
                            }
                            s->handle_close(sigEventIdx);
                        }
                    }
                }
            }
            //ReleaseMutex(s->gameLock);
        }
    }
    catch (logic_error& e) {
        cout << "Cannot handle exception:" << endl;
        cout << e.what();
        if (s) {
            s->error = 1;
            s->CloseRoom();
        }
    }
	//WSACleanup();
	return 0;
}

int Server::handle_close(int index){
    int i;
    if (index >= num_Members) {
        throw ceStateEx();
    }
    WSACloseEvent(Member_Event[index]);
    closesocket(Member_Sock[index]);

    for (i = 0; i < num_Players; i++) {
        if (index == PlayerSortedSock[i])break;
    }
    if (i >= num_Players) { 
        throw ceStateEx(); 
    }
    PlayerSortedSock[i] = 0;

    num_Members--;
    num_left--;

    for(i=index;i<num_Members;i++){
        buffer[i]=buffer[i+1];
        Member_Sock[i]=Member_Sock[i+1];
        Member_Event[i]=Member_Event[i+1];
    }

    for (i = 0; i < num_Players; i++) {
        if (PlayerSortedSock[i] > index) {
            PlayerSortedSock[i]--;
        }
    }

    if (num_left == 0 && num_Members > 1) {
        num_Players = 0;
        for (i = 1; i < num_Members; i++) {
            PlayerSortedSock[num_Players++] = i;
        }
    }
    else
    if (num_Members <=1) {
        CloseRoom();
    }
    if (state == GamePort::GAME_ON) {
        throw ceStateEx();
    }
    return 0;
}

int Server::BufClear(int index){
    queue<Package>empty;
    swap(empty, buffer[index]);
    return 0;
}

int Server::handle_accept(){
    SOCKET hClntSock;
    WSAEVENT clntEvent;
    SOCKADDR_IN clntAddr;
    int clntAdrSize;

    clntAdrSize = sizeof(clntAddr);
    hClntSock = accept(Member_Sock[0], (SOCKADDR*)&clntAddr, &clntAdrSize);

    if(num_Members>=humans+1){
        //GameMessage msg(CONN_DENIED);
        //SendGameInfo(msg,hClntSock);

        //发送拒绝受理
        SendSignal(ConnMsg:: MSG_CONN_DENY, hClntSock);
        closesocket(hClntSock);
        return 0;
    }

    clntEvent = WSACreateEvent();
    WSAEventSelect(hClntSock, clntEvent, FD_READ | FD_CLOSE);

    Member_Event[num_Members] = clntEvent;
    Member_Sock[num_Members] = hClntSock;
    BufClear(num_Members);
    num_Members++;

    return 0;
}

int Server::handle_read(int index){
    GameMessage msg;
    Package pkg(Header(false, i_server),"");
    int i;
    try {
        switch (read_msg(Member_Sock[index], msg, pkg)) {
        case ConnMsg::MSG_PAKAGE:
            if (state != GamePort::GAME_ON) {
                throw ceStateEx();
            }
            WaitForSingleObject(Mutex[index - 1], INFINITE);
            buffer[index].push(pkg);
            ReleaseMutex(Mutex[index - 1]);
            break;
        case ConnMsg::MSG_SEND_NAME:
            if (state != GamePort::GAME_OPEN) {
                throw ceStateEx();
            }
            if(num_left==0)
                PlayerSortedSock[num_Players++] = index;
            if (num_Players >= humans && num_left==0) {
                num_left = num_Players;
                for (int i = 0; i < robots; i++) {
                    string rob_name("robots_");
                    rob_name.append(1, '0' + i);
                    names.push_back(rob_name);
                }
                for (int i = 0; i < humans; i++) {
                    if (PlayerSortedSock[i]) {
                        for (int j = 0; j < humans + robots; j++) {
                            SendName(names[j], getSock(i));
                        }
                        SendGameType(game_t, getSock(i));
                        SendInt(i, getSock(i));
                        SendSignal(
                            ConnMsg::MSG_CONN_ACCEPT,
                            getSock(i)
                        );
                    }
                }
                names.clear();
                ready = 1;
            }
            break;
        case ConnMsg::MSG_WANT_CLOSE:
            while(state == GamePort::GAME_ON) {
            }
            ready = 0;

            SendSignal(ConnMsg::MSG_GRANT_CLOSE, Member_Sock[index]);
            //num_Players--;
            break;
        }
    }
    catch (ceReadInt& e) {
        cout << "the client closed socket in a way that is not normal" << endl;
    }
}

int Server:: OpenRoom(GameType gt, int humans, int robots){
    unsigned threadID;
    HANDLE hThread;
	WSADATA wsaData;
    WSAEVENT newEvent;

    // room cannot be openned again until
    // the lock is released (released in closeroom)
    //WaitForSingleObject(gameLock, INFINITE);

    if(state!= GamePort::GAME_OVER||error) return 1;//false
    state= GamePort::GAME_OPEN;
    ready = 0;
    num_left = 0;

    try {
        game_t = gt;
        this->humans = humans;
        this->robots = robots;

        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
            throw ceNetEx("WSAStartup() error!");

        Member_Sock[0] = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (Member_Sock[0] == INVALID_SOCKET)
            throw ceNetEx("socket() error!");

        addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(Member_Sock[0], (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
            throw ceNetEx("bind() error!");

        if (listen(Member_Sock[0], 5) == SOCKET_ERROR)
            throw ceNetEx("listen() error!");

        newEvent = WSACreateEvent();

        if (WSAEventSelect(Member_Sock[0], newEvent, FD_ACCEPT) == SOCKET_ERROR) {
            throw ceNetEx("WSAEventSelect() error");
        }

        Member_Event[0] = newEvent;
        num_Members = 1;
        num_Players = 0;

        // construct game
        //setGame();

        hThread = (HANDLE)_beginthreadex(
            NULL,
            0,
            thread_server_main,
            (void*)this,
            0,
            &threadID
        );
    }
    catch (logic_error& e) {
        cout << e.what();
        return 1;
    }
    //ReleaseMutex(gameLock);

    //while (!ready);
    return 0;
}

int Server:: CloseRoom(){
    if(state==GamePort::GAME_OVER) return 0;
    state = GamePort::GAME_OVER;

    ready = 0;
    // tell client to quit
    closesocket(Member_Sock[0]);
    WSACloseEvent(Member_Event[0]);
    num_Members=0;
    WSACleanup();

    return 0;
}

Server::Server():
    Port(),
    num_Members(0),
    humans(0),
    robots(0),
    num_Players(0),
    ready(0),
    num_left(0)
{
    int i;
    //gameLock = CreateMutex(NULL, FALSE, NULL);
    for(i=0;i<MAX_PLAYERS;i++){
        Mutex[i]=CreateMutex(NULL,FALSE,NULL);
        PlayerSortedSock[i] = 0;
    }
}

Server::~Server(){
    int i;
    //CloseRoom();
    for(i=0;i<MAX_PLAYERS;i++){
        CloseHandle(Mutex[i]);
    }
    //CloseHandle(gameLock);
}

//inline int Server:: NumPlayersInRoom()const{
//    return num_Members-1;
//}

//int Server::StartGame() { // branch error
//    state = GamePort::GAME_ON;
//
//    test_game(*this);
//
//    state = GamePort::GAME_OPEN;
//    return 0;
//}

//void test_game(Server& s) {
//    Package pkg(Header(0, 0));
//    cout << "game start" << endl;
//    pkg = Package(Header(1, 0),"Hello");
//    s.SendGameMsg(pkg);
//    pkg=s.CollectGameMsg(0);
//    if (pkg.GetHeader().IsSuccess()) {
//        cout << pkg.GetData();
//    }
//    else {
//        cout << "receive error" << endl;
//    }
//    cout << "game over" << endl;
//}

//void Server::setGame() { // branch error
//
//}