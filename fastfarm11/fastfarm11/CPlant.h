#ifndef _CPLANT_H_
#define _CPLANT_H_
#include "CEntity.h"

class CPlant : public CEntity {
public:
    bool clickHold;

public:
    //static std::vector<CEntity*>    EntityList;
    static int farm[FARM_HEIGHT][FARM_WIDTH];

protected:
    //CAnimation      Anim_Control;

    static SDL_Texture*    tex;

    /*public:
    float           X;
    float           Y;

    int             Width;
    int             Height;

    int				scaleW;
    int				scaleH;

    int             AnimState;
    */
public:
    CPlant();

    //~CPlant();

public:
    bool OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW = -1, int scaleH=-1);

    static int OnLoopStatic();

    void OnRender(SDL_Renderer* ren);

    static void OnCleanupStatic();
    void OnEvent(SDL_Event* Event);
    void OnLoop();
};

#endif