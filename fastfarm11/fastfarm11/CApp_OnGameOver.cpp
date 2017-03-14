#include "CApp.h"

void CApp::OnGameOver()
{
    bool win=true;
    SDL_Texture* background;
    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++)
    {
        for (int j = FARM_j; j < FARM_WIDTH+FARM_j; j++)
        {
            if(CPlant::farm[i-FARM_i][j-FARM_j]==2 || CPlant::farm[i-FARM_i][j-FARM_j]==1 ) {win=false; break;}
        }
    }

    SDL_Event e;
    if(win)
    {   
        int score = 10000+(float)(10*CSeedlingW::no_seedling) + (float)1000/(CSeedlingW::moves*CSeedlingW::moves)+(float)1000/(CSeedlingW::seedling_purch*CSeedlingW::seedling_purch);

        char str[250];
        CArea::AreaControl.OnRender(ren,-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());

        sprintf(str,"Purchases made = %d\nMoves Taken = %d\nStock Retained = %d\nYour Score = %d\n Can it be better???",CSeedlingW::seedling_purch,CSeedlingW::moves,CSeedlingW::no_seedling,score);

        SDL_Texture* textTex = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 28,ren,600);
        background = CSurface::OnLoad("../tilesets/frame.png",ren);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-3)*TILE_SIZE - TILE_SIZE/2, (MAP_HEIGHT/2-4)*TILE_SIZE);
        CSurface::OnDraw(textTex,ren,(MAP_WIDTH/2-2)*TILE_SIZE,(MAP_HEIGHT/2-2)*TILE_SIZE);
        SDL_RenderPresent(ren);
        SDL_DestroyTexture(textTex);  textTex = NULL;
        SDL_DestroyTexture(background);  background = NULL;
        Running=2;
        wait_key_click();
    }

}