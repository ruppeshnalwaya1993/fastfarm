#ifndef _CENTITY_H_
#define _CENTITY_H_

#include <vector> 
#include <utility>
#include "CAnimation.h"
#include "CSurface.h"
#include "CArea.h"

class CEntity {
public:
    static std::vector<CEntity*>    EntityList;

protected:
    CAnimation      Anim_Control;

    SDL_Texture*    tex;

public:
    float           X;
    float           Y;

    int             Width;
    int             Height;

    int				scaleW;
    int				scaleH;

    int             AnimState;
    int deadstate;
    bool alive;
    bool dead;

public:
    CEntity();

    virtual ~CEntity();

public:
    virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW = -1, int scaleH=-1);

    virtual void OnLoop();

    virtual void OnRender(SDL_Renderer* ren);

    virtual void OnCleanup();

    virtual void OnEvent(SDL_Event* Event);
    virtual std::pair<float, float> getCenter();
};

#endif