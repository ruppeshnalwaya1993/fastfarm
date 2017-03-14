#include "CEntity.h"

std::vector<CEntity*> CEntity::EntityList;

CEntity::CEntity() {
    tex = NULL;

    X = Y = 0.0f;

    scaleW = -1;
    scaleH = -1;

    Width = Height = 0;

    AnimState = 0;

    alive = true;
    dead = false;
    deadstate = 0;
}

CEntity::~CEntity() {
}

bool CEntity::OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW , int scaleH) {
    if((tex = CSurface::OnLoad(File,ren)) == NULL) {
        return false;
    }

    //CSurface::Transparent(Surf_Entity, 255, 0, 255);

    this->Width = Width;
    this->Height = Height;
    this->scaleW = scaleW;
    this->scaleH = scaleH;

    Anim_Control.MaxFrames = MaxFrames;

    return true;
}

void CEntity::OnLoop() {
    /*scaleH = CSurface::scalar*Height;
    scaleW = CSurface::scalar*Width;*/
    Anim_Control.OnAnimate();
    if (alive == false)
    {
        if(AnimState!=deadstate)
        {
            AnimState =deadstate;
            Anim_Control.wait_time=0;
            Anim_Control.SetCurrentFrame(0);
        }
        else if(Anim_Control.GetCurrentFrame()==Anim_Control.MaxFrames-1) {
            dead = true; 
        }

    }
}

void CEntity::OnRender(SDL_Renderer* ren) {
    if(tex == NULL || ren == NULL) 
        return;

    CSurface::OnDraw(tex, ren, X, Y, AnimState * Width, Anim_Control.GetCurrentFrame() * Height, Width, Height, scaleW, scaleH);
}

void CEntity::OnCleanup() {
    if(tex) {
        SDL_DestroyTexture(tex);
    }

    tex = NULL;
}

void CEntity::OnEvent(SDL_Event* Event)
{}

std::pair<float,float> CEntity::getCenter()
{
    pair<int,int> p;
    p.first = Width/2;
    p.second= Height/2;
    return p;
}