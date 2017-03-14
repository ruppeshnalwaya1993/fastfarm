//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnRender() {
    SDL_RenderClear(ren);

    CArea::AreaControl.OnRender(ren, -CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
    CSurface::OnDraw(textTex, ren, (0)*TILE_SIZE+10, 10);

    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) continue;

        CEntity::EntityList[i]->OnRender(ren);
    }

    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++) {
        for (int j = FARM_j; j < FARM_WIDTH+FARM_j; j++) {
            if(CPlant::farm[i-FARM_i][j-FARM_j]==3) {
                //printf("ok");
                CSurface::OnDraw(crop, ren, j*TILE_SIZE, i*TILE_SIZE);
            }
        }
    }

    SDL_RenderPresent(ren);
}

//==============================================================================