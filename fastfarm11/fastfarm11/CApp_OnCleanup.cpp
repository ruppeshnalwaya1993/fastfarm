//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnCleanup() {
    SDL_DestroyTexture(textTex);  textTex=NULL;
    //SDL_DestroyRenderer(ren); 
    //SDL_DestroyWindow(window);  

    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++)
    {
        for (int j = FARM_j; j < FARM_WIDTH; j++)
        {
            CPlant::farm[i][j]=0;
        }
    }

    for(int i = 0;i < CEntity::EntityList.size();i++) {
        if(!CEntity::EntityList[i]) 
            continue;

        CEntity::EntityList[i]->OnCleanup();
        delete CEntity::EntityList[i];
    }

    CEntity::EntityList.clear();
    SDL_DestroyTexture(crop); 
    crop =nullptr;
}

//==============================================================================
