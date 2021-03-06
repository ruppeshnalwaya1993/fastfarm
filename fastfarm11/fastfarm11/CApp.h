//==============================================================================
// SDL Tutorial 1
//==============================================================================
#ifndef _CAPP_H_
#define _CAPP_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "CSurface.h"
#include "CAppConstants.h"
#include "CEvent.h"
#include "CEntity.h"
#include "CArea.h"
#include "CCamera.h"
#include "CPlant.h"
#include "CSeedlingW.h"

//==============================================================================
class CApp : public CEvent{
private:
    int            Running;
    SDL_Window* window;
    SDL_Renderer*  ren;
    SDL_Texture*	textTex;
    SDL_Texture* crop;
    int level;
public:
    CApp(SDL_Renderer* ren1, SDL_Window* win1);

    int OnExecute();

public:
    bool OnInit();

    void OnEvent(SDL_Event* Event);

    void OnLoop();

    void OnRender();

    void OnCleanup();
    void OnExit();
    void OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat);
    void OnLButtonDown(int mX, int mY);
    void OnGameOver();

};

//==============================================================================

#endif
