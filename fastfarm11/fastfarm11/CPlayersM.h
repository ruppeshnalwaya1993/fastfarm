#ifndef _CPLAYERSM_H_
#define _CPLAYERSM_H_
#include "CPlantM.h"
#include "CTimer.h"

struct HistEve {
    int type;
    int fingerId;
};

enum {
    NORMAL=0, UNDER, OVER
};

class CPlayersM
{
private:
    CEntity pl1;
    CEntity pl2;
    int no_init_pop1;
    int no_init_pop2;


    int minPeriod;	// in minutes
    int maxPeriod;	// in minutes
    int initPeriod;  //in seconds
    HistEve recEvent1;
    HistEve recEvent2;
    SDL_Renderer* ren;

public: 
    CPlayersM();
    static CPlayersM playerControl;
    float ratio1_2;
    int count_pl1;
    int count_pl2;
    int gamePeriod;  //in minutes
    CTimer tim;

    void OnEvent(SDL_Event* Event);
    bool OnLoad(char* File1, char* File2, int Width, int Height, int MaxFrames, SDL_Renderer *ren);
    void OnRender(SDL_Renderer* ren);
    void OnLoop();
    void OnCleanup();
    void OnInit(SDL_Renderer* ren1, int &Running);
};

#endif