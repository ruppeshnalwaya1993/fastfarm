//==============================================================================
#include "CMenu.h"

//==============================================================================
CMenu::CMenu(SDL_Renderer* ren1, SDL_Window* win1) {
    win = win1;
    ren = ren1;
    Running = 1;
    menupng = NULL;
    ffgreet = NULL;
}

//------------------------------------------------------------------------------
int CMenu::OnExecute() {
    Running = 1;
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    while(Running==1) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return Running;
}

bool CMenu::OnInit() {
    if(CArea::AreaControl.OnLoad("../maps/1.area", ren) == false) {
        return false;
    }

    menupng = CSurface::OnLoad("../tilesets/menu.png",ren);
    ffgreet = CSurface::OnLoad("../tilesets/ffgreet.png",ren);
    return true;
}

void CMenu::OnLoop() {
}

void CMenu::OnRender() {
    CArea::AreaControl.OnRender(ren,-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
    CSurface::OnDraw(menupng,ren,(MAP_WIDTH/2-3)*TILE_SIZE - TILE_SIZE/2, (MAP_HEIGHT/2-4)*TILE_SIZE);
    CSurface::OnDraw(ffgreet,ren,0, TILE_SIZE);
    SDL_RenderPresent(ren);
}

void CMenu::OnCleanup() {
    CArea::AreaControl.OnCleanup();
    SDL_DestroyTexture(menupng);
    SDL_DestroyTexture(ffgreet);
}

void CMenu::OnExit() {
    Running = 0;
}

void CMenu::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat) {
    switch(sym) {
    case SDLK_UP:    CCamera::CameraControl.OnMove( 0,-5); break;
    case SDLK_DOWN:  CCamera::CameraControl.OnMove( 0, 5); break;
    case SDLK_LEFT:  CCamera::CameraControl.OnMove(-5, 0); break;
    case SDLK_RIGHT: CCamera::CameraControl.OnMove( 5, 0); break;
    case SDLK_ESCAPE: Running = 0;

    default: {
             }
    }
}

void CMenu::OnLButtonDown(int mX, int mY) {
    int i=(mY/CSurface::scaleY)/TILE_SIZE;
    int j=(mX/CSurface::scaleX)/TILE_SIZE;
    if(j>=MAP_WIDTH/2-1 && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2-3) && i<(MAP_HEIGHT/2 -2))
        Running =2;
    else if(j>=(MAP_WIDTH/2-1) && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2 -2) && i<(MAP_HEIGHT/2-1))
        Running=3;
    else if(j>=(MAP_WIDTH/2-1) && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2-1) && i<(MAP_HEIGHT/2))
        Running=4;
    else if(j>=(MAP_WIDTH/2-1) && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2) && i<(MAP_HEIGHT/2+1))
        Running=5;
    else if(j>=(MAP_WIDTH/2-1) && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2+1) && i<(MAP_HEIGHT/2+2))
        Running=6;
    else if(j>=(MAP_WIDTH/2-1) && j<(MAP_WIDTH/2+1) && i>=(MAP_HEIGHT/2+2) && i<(MAP_HEIGHT/2+3))
        Running=7;
    else if(mX >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && mX <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
        Running=0;
}

void CMenu::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}