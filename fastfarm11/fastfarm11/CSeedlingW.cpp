#include "CSeedlingW.h"

int CSeedlingW::no_seedling = 0;
int CSeedlingW::seedling_purch = 0;
int CSeedlingW::moves = 0;

CSeedlingW::CSeedlingW()
{
    //no_seedling = 9999;
    clickHold = false;
    plantC = NULL;
    ren = NULL;
    Anim_Control.Oscillate = true;
    Anim_Control.wait_time = 1000;
}

void CSeedlingW::OnEvent(SDL_Event* Event)
{
    switch(Event->type) {
    case SDL_MOUSEBUTTONDOWN: {
        switch(Event->button.button) {
        case SDL_BUTTON_LEFT: {
            if((Event->button.x)/CSurface::scaleX >= X && (Event->button.y)/CSurface::scaleY >= Y && (Event->button.x)/CSurface::scaleX < X+Width && (Event->button.y)/CSurface::scaleY < Y+Height) {
                ++no_seedling; 
                ++seedling_purch;
            }

            //X=(Event->button.x)/CSurface::scaleX-getCenter().first; Y=(Event->button.y)/CSurface::scaleY-getCenter().second;
            if(plantC == NULL && no_seedling>0 && ren!=NULL) {   
                //clickHold = true;
                int j=(Event->button.x/CSurface::scaleX)/TILE_SIZE;
                int i=(Event->button.y/CSurface::scaleY)/TILE_SIZE;
                printf("%d %d\n",i,j);
                if(CPlant::farm[i-FARM_i][j-FARM_j]>0 || i>=FARM_HEIGHT+FARM_i || j>=FARM_WIDTH+FARM_j || i<FARM_i||j<FARM_j) 
                    break;
                plantC = new CPlant;
                //plantC->clickHold = true;
                plantC->OnLoad("../tilesets/weedsheet.png", 82, 82, 8, ren);
                plantC->X = j*TILE_SIZE+TILE_SIZE/2-plantC->getCenter().first;
                plantC->Y = i*TILE_SIZE+TILE_SIZE/2-plantC->getCenter().second;
                CPlant::farm[i-FARM_i][j-FARM_j]=1;
                CEntity::EntityList.push_back(plantC);
                plantC = NULL;
                --no_seedling;
                ++moves;
            }
            //}
            break;
                              }
        }
        break;
                              }

    case SDL_MOUSEBUTTONUP:    {/*
        switch(Event->button.button) {
        case SDL_BUTTON_LEFT: {
            if(clickHold) {
                clickHold = false;
                int i,j;
                j = ((Event->button.x)/CSurface::scaleX)/TILE_SIZE;
                i = ((Event->button.y)/CSurface::scaleY)/TILE_SIZE;
                if(i<MAP_HEIGHT && j<MAP_WIDTH-4) {
                    if(CPlant::farm[i][j]==0) {
                        CPlant::farm[i][j] = 1;
                        printf("%d %d\n",i,j);
                        plantC->clickHold = 0;
                        plantC->X = j*TILE_SIZE+TILE_SIZE/2 - plantC->getCenter().first;
                        plantC->Y = i*TILE_SIZE+TILE_SIZE/2 - plantC->getCenter().second;
                        CEntity::EntityList.push_back(plantC);
                        plantC = NULL;
                    }
                    else {
                        ++no_seedling;
                        plantC->OnCleanup();
                        delete plantC;
                        plantC = NULL;
                    }
                }
                else {
                    ++no_seedling;
                    plantC->OnCleanup();
                    delete plantC;
                    plantC = NULL;
                }

            }
            break;
                              }
        }
        break;*/
                               }

    case SDL_MOUSEMOTION: {/*
        if(clickHold) {
            plantC->X=(Event->button.x)/CSurface::scaleX-plantC->getCenter().first; 
            plantC->Y=(Event->button.y)/CSurface::scaleY-plantC->getCenter().second;
        }
        break;*/
                          }
    }
}

//------------------------------------------------------------------------------------------
void CSeedlingW::OnRender(SDL_Renderer* ren)
{
    this->CEntity::OnRender(ren);
    this->ren = ren; 

    if(plantC!=NULL)
        plantC->OnRender(ren);
}

//--------------------------------------------------------------------------------------------
void CSeedlingW::OnLoop()
{
    this->CEntity::OnLoop();
    if(plantC!=NULL)
        plantC->OnLoop();
}

//--------------------------------------------------------------------------------------------
void CSeedlingW::OnCleanup()
{
    this->CEntity::OnCleanup();
    if(plantC!=NULL)
    {plantC->OnCleanup();
    delete plantC;
    }
    plantC = NULL;
}