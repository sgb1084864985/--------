#ifndef _GAME_LAUNCHER_H
#define _GAME_LAUNCHER_H
#include "package.h"
#include "Server.h"
#include "ConnectBase.h"
#include "game.h"

class GameLauncher
{
public:
    // return: success if 0 else failure occured
    int OpenRoom(GameType gt, int humans, int robots);

    // return: success if 0 else failure occured
    virtual int StartGame();

    GameLauncher():s(),game_ptr(nullptr){}
    ~GameLauncher();
protected:
    void* game_ptr;
    Server s;
    virtual void setGame();
    virtual void clearGame();
    static unsigned WINAPI thread_main(void* LPgamelauncher);
};

#endif