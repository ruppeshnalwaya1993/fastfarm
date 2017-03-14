//==============================================================================
#include "CMulti.h"

//==============================================================================
CMulti::CMulti(SDL_Renderer* ren1, SDL_Window* win1) {
    window = win1;
    ren = ren1;
    Running = 1;
    textTex = nullptr;
    level =1;
    crop = nullptr;
    barTex = NULL;
}

//------------------------------------------------------------------------------
int CMulti::OnExecute() {
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
        OnGameOver();
    }

    OnCleanup();

    return Running;
}

bool CMulti::OnInit()
{
    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++) {
        for (int j = FARM_j; j < FARM_WIDTH+FARM_j; j++) {
            CPlant::farm[i-FARM_i][j-FARM_j]=0;
        }
    }
    CPlayersM::playerControl.OnInit(ren,Running);
    if(CArea::AreaControl.OnLoad("../maps/1.area", ren) == false) {
        return false;
    }
    if(CPlayersM::playerControl.OnLoad("../tilesets/pl1.png","../tilesets/pl2.png",82,82,1,ren)==false) 
        return false;

    ratioEnt.OnLoad("../tilesets/ratio.png",37,37,1,ren);
    ratioEnt.X= (FARM_j+FARM_WIDTH/2)*TILE_SIZE - ratioEnt.getCenter().first;
    ratioEnt.Y= (FARM_i)*TILE_SIZE-ratioEnt.getCenter().second;
    barTex = CSurface::OnLoad("../tilesets/ratio.png",ren);

    return true;
}

void CMulti::OnLoop() {
    CPlayersM::playerControl.OnLoop();
    CPlantM::OnLoopStatic();

    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) 
            continue;

        CEntity::EntityList[i]->OnLoop();

        if(CEntity::EntityList[i]->dead) {
            CEntity::EntityList[i]->OnCleanup();
            delete CEntity::EntityList[i];
            CEntity::EntityList[i] = NULL;
            CEntity::EntityList.erase(CEntity::EntityList.begin()+i);
            continue;
        }
    }

    if(CPlayersM::playerControl.count_pl1 != CPlayersM::playerControl.count_pl2)
    {
        int c1 =CPlayersM::playerControl.count_pl1, c2=CPlayersM::playerControl.count_pl2;
        /*if(CPlayersM::playerControl.count_pl1 > CPlayersM::playerControl.count_pl2)
        ratioEnt.X= ((FARM_j+FARM_WIDTH/2)*TILE_SIZE) - ( ((c1+1)*(c1+1)*492)/((c1+c2+2)*(c1+c2+2)*2) ) - ratioEnt.getCenter().first;
        else*/ ratioEnt.X= ((FARM_j+FARM_WIDTH/2)*TILE_SIZE) + ( ((c2-c1)*492)/((c1+c2+2)*2) ) - ratioEnt.getCenter().first;
    }
    else { 
        ratioEnt.X= (FARM_j+FARM_WIDTH/2)*TILE_SIZE - ratioEnt.getCenter().first;
    }
}

void CMulti::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
    CPlayersM::playerControl.OnEvent(Event);
}

void CMulti::OnRender() {
    CArea::AreaControl.OnRender(ren, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
    CPlayersM::playerControl.OnRender(ren);
    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) 
            continue;

        CEntity::EntityList[i]->OnRender(ren);
    }
    CSurface::OnDraw(barTex,ren,(FARM_j+FARM_WIDTH/2-3)*TILE_SIZE,(FARM_i)*TILE_SIZE-4,0,37,492,8);
    ratioEnt.OnRender(ren);
    SDL_RenderPresent(ren);
}

void CMulti::OnCleanup() {
    CPlayersM::playerControl.OnCleanup();
    SDL_DestroyTexture(barTex); barTex = NULL;
    ratioEnt.OnCleanup();
    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) 
            continue;

        CEntity::EntityList[i]->OnCleanup();
        delete CEntity::EntityList[i];
    }
    CEntity::EntityList.clear();
}

void CMulti::OnGameOver() {
    if(CPlayersM::playerControl.tim.Ticks()/1000>=CPlayersM::playerControl.gamePeriod*60) {
        SDL_Texture* overGreet;
        SDL_Texture* background;
        background = CSurface::OnLoad("../tilesets/frame.png",ren);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-3)*TILE_SIZE - TILE_SIZE/2, (MAP_HEIGHT/2-4)*TILE_SIZE);
        if(CPlayersM::playerControl.ratio1_2>1)
            overGreet= CSurface::OnLoadText("Player 1 \nWins","../CooperBlackStd.otf", 110,236,4, 28,ren,400);
        else if(CPlayersM::playerControl.ratio1_2<1)
            overGreet= CSurface::OnLoadText("Player 2 \nwins","../CooperBlackStd.otf", 110,236,4, 28,ren,400);
        else overGreet= CSurface::OnLoadText("Draw","../CooperBlackStd.otf", 110,236,4, 28,ren,400);

        CSurface::OnDraw(overGreet,ren,(MAP_WIDTH/2-1)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE);
        SDL_RenderPresent(ren);
        wait_key_click();
        SDL_DestroyTexture(overGreet);
        SDL_DestroyTexture(background);

        Running =2;

        CPlayersM::playerControl.tim.Stop();
    }
}

void CMulti::OnExit() {
    Running = 2;
}

void CMulti::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat) {
    switch(sym) {
    case SDLK_ESCAPE: Running = 2;
        break;

    default: {
             }
    }
}

void CMulti::OnLButtonDown(int mX, int mY) {
    printf("mouse: %d %d\n", mX,mY);
    int i=(mY/CSurface::scaleY)/TILE_SIZE;
    int j=(mX/CSurface::scaleX)/TILE_SIZE;
    if(mX >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && mX <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
        Running=2;
}
