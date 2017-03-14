#include "CPlayersM.h"
#include "CCamera.h"

CPlayersM CPlayersM::playerControl;

CPlayersM::CPlayersM()
{
    pl1.X=0; 
    pl1.Y=1*TILE_SIZE;
    pl2.X=(MAP_WIDTH-1)*TILE_SIZE;  
    pl2.Y=1*TILE_SIZE;
    no_init_pop1=10; 
    no_init_pop2=10;
    gamePeriod=15;
    initPeriod=30;
    minPeriod=2;
    maxPeriod = 30;
    ratio1_2 = 1;
    count_pl1 =0;
    count_pl2 =0;
    recEvent1.fingerId=-1;
    recEvent1.type=-1;
    recEvent2.fingerId=-1;
    recEvent2.type=-1;
    ren =NULL;
}

bool CPlayersM::OnLoad(char* File1, char* File2, int Width, int Height, int MaxFrames, SDL_Renderer *ren) {
    return pl1.OnLoad(File1,Width,Height,MaxFrames,ren) & pl2.OnLoad(File2,Width,Height,MaxFrames,ren);
}

void CPlayersM::OnLoop()
{
    if(!tim.Started()) 
        tim.Start();
    if(no_init_pop1==0 && tim.Ticks()/1000>initPeriod) {
        if(ratio1_2>1) 
            pl1.AnimState=2;
        else if(ratio1_2<1) 
            pl1.AnimState=1;
        else pl1.AnimState=0;
    }

    if(no_init_pop2==0 && tim.Ticks()/1000>initPeriod) {
        if(ratio1_2>1) 
            pl2.AnimState=1;
        else if(ratio1_2<1) 
            pl2.AnimState=2;
        else 
            pl2.AnimState=0;
    }
    pl1.OnLoop();	
    pl2.OnLoop();
}

void CPlayersM::OnEvent(SDL_Event* Event) {
    int pl1id = pl1.X/TILE_SIZE + (pl1.Y/TILE_SIZE)*MAP_WIDTH;
    int pl2id = pl2.X/TILE_SIZE + (pl2.Y/TILE_SIZE)*MAP_WIDTH;

    switch(Event->type) {
    case SDL_FINGERDOWN: {
        int tileid = (int)(Event->tfinger.x*SCREEN_WIDTH)/TILE_SIZE + ((int)(Event->tfinger.y*SCREEN_HEIGHT)/TILE_SIZE)*MAP_WIDTH;
        //printf("tileid %d\n x %f y %f\n",tileid,Event->tfinger.x*CSurface::scaleX*SCREEN_WIDTH,Event->tfinger.y*CSurface::scaleY*SCREEN_HEIGHT);
        if(tileid==pl1id) {
            if(recEvent1.fingerId==-1 || recEvent1.type==SDL_FINGERUP) {
                recEvent1.fingerId = Event->tfinger.fingerId;
                recEvent1.type = Event->type;
            }
        }
        else if(tileid==pl2id) {
            if(recEvent2.fingerId==-1 || recEvent2.type==SDL_FINGERUP) {
                recEvent2.fingerId = Event->tfinger.fingerId;
                recEvent2.type = Event->type;
            }
        }
        break;
                         }

    case SDL_FINGERMOTION:	{
        if(Event->tfinger.fingerId==recEvent1.fingerId && (recEvent1.type==SDL_FINGERDOWN || recEvent1.type==SDL_FINGERMOTION)) {
            int x = ((int)(Event->tfinger.x*SCREEN_WIDTH)/TILE_SIZE)*TILE_SIZE;
            int y = ((int)(Event->tfinger.y*SCREEN_HEIGHT)/TILE_SIZE)*TILE_SIZE;
            if(x/TILE_SIZE>=FARM_j && x/TILE_SIZE<FARM_j+FARM_WIDTH && y/TILE_SIZE>=FARM_i && y/TILE_SIZE<FARM_HEIGHT+FARM_i) {
                if(x!=pl2.X || y!=pl2.Y) {
                    pl1.X = x;  pl1.Y = y;
                }
            }
            recEvent1.fingerId = Event->tfinger.fingerId;
            recEvent1.type = Event->type;
        }
        else if(Event->tfinger.fingerId==recEvent2.fingerId && (recEvent2.type==SDL_FINGERDOWN || recEvent2.type==SDL_FINGERMOTION)) {
            int x = ((int)(Event->tfinger.x*SCREEN_WIDTH)/TILE_SIZE)*TILE_SIZE;
            int y = ((int)(Event->tfinger.y*SCREEN_HEIGHT)/TILE_SIZE)*TILE_SIZE;
            if(x/TILE_SIZE>=FARM_j && x/TILE_SIZE<FARM_j+FARM_WIDTH && y/TILE_SIZE>=FARM_i && y/TILE_SIZE<FARM_HEIGHT+FARM_i) {
                if(x!=pl1.X || y!=pl1.Y) {
                    pl2.X = x;  pl2.Y = y;
                }
            }
            recEvent2.fingerId = Event->tfinger.fingerId;
            recEvent2.type = Event->type;
        }
        break;
                            }

    case SDL_FINGERUP:	{
        printf("touchup x %f y %f\n",Event->tfinger.x*CSurface::scaleX*SCREEN_WIDTH,Event->tfinger.y*CSurface::scaleY*SCREEN_HEIGHT);
        if(Event->tfinger.fingerId==recEvent1.fingerId && (recEvent1.type==SDL_FINGERDOWN || recEvent1.type==SDL_FINGERMOTION)) {
            if(recEvent1.type==SDL_FINGERDOWN) {
                //take action like planting
                if(CPlant::farm[(int)pl1.Y/TILE_SIZE-FARM_i][(int)pl1.X/TILE_SIZE-FARM_j]) 
                    break;
                if(no_init_pop1>0) {
                    --no_init_pop1; 
                    goto lowf1;
                }
                else if(tim.Ticks()/1000<initPeriod) { 
                    goto normf1;
                }
                else if(ratio1_2>1) {
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = OVER1-1;
                    plantC->deadstate = DYING1-1;
                    plantC->X = pl1.X;
                    plantC->Y = pl1.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl1.X/TILE_SIZE;
                    int i = pl1.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=OVER1;
                }
                else if(ratio1_2==1) {
normf1:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = NORMAL1-1;
                    plantC->deadstate = DYING1-1;
                    plantC->X = pl1.X;
                    plantC->Y = pl1.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl1.X/TILE_SIZE;
                    int i = pl1.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=NORMAL1;
                }
                else {
lowf1:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = UNDER1-1;
                    plantC->deadstate = DYING1-1;
                    plantC->X = pl1.X;
                    plantC->Y = pl1.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl1.X/TILE_SIZE;
                    int i = pl1.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=UNDER1;
                }
            }
            recEvent1.fingerId = Event->tfinger.fingerId;
            recEvent1.type = Event->type;
        }
        else if(Event->tfinger.fingerId==recEvent2.fingerId && (recEvent2.type==SDL_FINGERDOWN || recEvent2.type==SDL_FINGERMOTION)) {
            if(recEvent2.type==SDL_FINGERDOWN) {
                //take action like planting
                if(CPlant::farm[(int)pl2.Y/TILE_SIZE-FARM_i][(int)pl2.X/TILE_SIZE-FARM_j]) 
                    break;
                if(no_init_pop2>0) {
                    --no_init_pop2; 
                    goto lowf2;
                }
                else if(tim.Ticks()/1000<initPeriod) { 
                    goto normf2;
                }
                else if(ratio1_2<1) {
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = OVER2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=OVER2;
                }
                else if(ratio1_2==1) {
normf2:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = NORMAL2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=NORMAL2;
                }
                else {
lowf2:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = UNDER2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=UNDER2;
                }
            }
            recEvent2.fingerId = Event->tfinger.fingerId;
            recEvent2.type = Event->type;
        }
        break;
                        }

    case SDL_KEYDOWN:	{ 
        if(Event->key.repeat) 
            break;
        switch(Event->key.keysym.sym) {
        case SDLK_w : {
            if(pl1.Y-TILE_SIZE>=1 && (pl1.Y-TILE_SIZE!=pl2.Y || pl1.X!=pl2.X))
                pl1.Y -=TILE_SIZE;
            break;
                      }
        case SDLK_a : {
            if(pl1.X-TILE_SIZE>=0 && (pl1.Y!=pl2.Y || pl1.X-TILE_SIZE!=pl2.X))
                pl1.X -=TILE_SIZE;
            break;
                      }
        case SDLK_s : {
            if(pl1.Y+TILE_SIZE <= (MAP_HEIGHT-1)*TILE_SIZE && (pl1.Y+TILE_SIZE!=pl2.Y || pl1.X!=pl2.X))
                pl1.Y +=TILE_SIZE;
            break;
                      }
        case SDLK_d : {
            if(pl1.X+TILE_SIZE <= (MAP_WIDTH-1)*TILE_SIZE && (pl1.Y!=pl2.Y || pl1.X+TILE_SIZE!=pl2.X))
                pl1.X +=TILE_SIZE;
            break;
                      }
        case SDLK_LCTRL : {
            // take action like planting
            if(CPlant::farm[(int)pl1.Y/TILE_SIZE-FARM_i][(int)pl1.X/TILE_SIZE-FARM_j]) 
                break;
            if(no_init_pop1>0) {
                --no_init_pop1; 
                goto lowk1;
            }
            else if(tim.Ticks()/1000<initPeriod) { 
                goto normk1;
            }
            else if(ratio1_2>1) {
                CEntity* plantC = new CPlantM();
                plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                plantC->AnimState = OVER1-1;
                plantC->deadstate = DYING1-1;
                plantC->X = pl1.X;
                plantC->Y = pl1.Y;
                CEntity::EntityList.push_back(plantC);
                int j = pl1.X/TILE_SIZE;
                int i = pl1.Y/TILE_SIZE;
                CPlant::farm[i-FARM_i][j-FARM_j]=OVER1;
            }
            else if(ratio1_2==1) {
normk1:
                CEntity* plantC = new CPlantM();
                plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                plantC->AnimState = NORMAL1-1;
                plantC->deadstate = DYING1-1;
                plantC->X = pl1.X;
                plantC->Y = pl1.Y;
                CEntity::EntityList.push_back(plantC);
                int j = pl1.X/TILE_SIZE;
                int i = pl1.Y/TILE_SIZE;
                CPlant::farm[i-FARM_i][j-FARM_j]=NORMAL1;
            }
            else {
lowk1:
                CEntity* plantC = new CPlantM();
                plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                plantC->AnimState = UNDER1-1;
                plantC->deadstate = DYING1-1;
                plantC->X = pl1.X;
                plantC->Y = pl1.Y;
                CEntity::EntityList.push_back(plantC);
                int j = pl1.X/TILE_SIZE;
                int i = pl1.Y/TILE_SIZE;
                CPlant::farm[i-FARM_i][j-FARM_j]=UNDER1;
            }
            break;
                          }
        case SDLK_UP : {
            if(pl2.Y-TILE_SIZE>=1 && (pl2.Y-TILE_SIZE!=pl1.Y || pl2.X!=pl1.X))
                pl2.Y -=TILE_SIZE;
            break;
                       }
        case SDLK_LEFT : {
            if(pl2.X-TILE_SIZE>=0 && (pl2.Y!=pl1.Y || pl2.X-TILE_SIZE!=pl1.X))
                pl2.X -=TILE_SIZE;
            break;
                         }
        case SDLK_DOWN : {
            if(pl2.Y+TILE_SIZE <= (MAP_HEIGHT-1)*TILE_SIZE && (pl2.Y+TILE_SIZE!=pl1.Y || pl2.X!=pl1.X))
                pl2.Y +=TILE_SIZE;
            break;
                         }
        case SDLK_RIGHT : {
            if(pl2.X+TILE_SIZE <= (MAP_WIDTH-1)*TILE_SIZE && (pl2.Y!=pl1.Y || pl2.X+TILE_SIZE!=pl1.X))
                pl2.X +=TILE_SIZE;
            break;
                          }
        case SDLK_RCTRL : {
            // take action like planting
            if(CPlant::farm[(int)pl2.Y/TILE_SIZE-FARM_i][(int)pl2.X/TILE_SIZE-FARM_j]) 
                break;
            if(no_init_pop2>0) {
                --no_init_pop2; 
                goto lowk2;
            }
            else
                if(tim.Ticks()/1000<initPeriod){ 
                    goto normk2;
                }
                else if(ratio1_2<1) {
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = OVER2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=OVER2;
                }
                else if(ratio1_2==1) {
normk2:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = NORMAL2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=NORMAL2;
                }
                else {
lowk2:
                    CEntity* plantC = new CPlantM();
                    plantC->OnLoad("../tilesets/farmsheet2.png",82,82,8,ren);
                    plantC->AnimState = UNDER2-1;
                    plantC->deadstate = DYING2-1;
                    plantC->X = pl2.X;
                    plantC->Y = pl2.Y;
                    CEntity::EntityList.push_back(plantC);
                    int j = pl2.X/TILE_SIZE;
                    int i = pl2.Y/TILE_SIZE;
                    CPlant::farm[i-FARM_i][j-FARM_j]=UNDER2;
                }
                break;
                          }
        }
        break;
                        }
    }
}

void CPlayersM::OnRender(SDL_Renderer* ren) {
    pl1.OnRender(ren); pl2.OnRender(ren);
    SDL_Texture* tempRatio;
    try {
        char str[300];
        if(tim.Ticks()/1000<60)
            sprintf(str,"Ratio %0.3f         Timer %d sec",ratio1_2,tim.Ticks()/1000);
        else
            sprintf(str,"Ratio %0.3f         Timer %d min %d sec",ratio1_2,tim.Ticks()/(60*1000),(tim.Ticks()/1000)%60);
        tempRatio = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 22,ren,1000);
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    }
    CSurface::OnDraw(tempRatio,ren,(MAP_WIDTH/2-2)*TILE_SIZE,TILE_SIZE/2);
    SDL_DestroyTexture(tempRatio);  tempRatio = NULL;
    tempRatio = CSurface::OnLoadText("Player 1", "../CooperBlackStd.otf", 110,236,4, 28,ren,1000);
    CSurface::OnDraw(tempRatio,ren,TILE_SIZE/2,TILE_SIZE/2-10);
    SDL_DestroyTexture(tempRatio);  tempRatio = NULL;
    tempRatio = CSurface::OnLoadText("Player 2", "../CooperBlackStd.otf", 110,236,4, 28,ren,1000);
    CSurface::OnDraw(tempRatio,ren,(MAP_WIDTH-2)*TILE_SIZE,TILE_SIZE/2-10);
    SDL_DestroyTexture(tempRatio);  tempRatio = NULL;
}

void CPlayersM::OnInit(SDL_Renderer* ren1, int &Running)
{
    pl1.X=0; pl1.Y=1*TILE_SIZE;
    pl2.X=(MAP_WIDTH-1)*TILE_SIZE;  pl2.Y=1*TILE_SIZE;
    pl1.AnimState=0;
    pl2.AnimState=0;
    pl1.alive=true;
    pl2.alive=true;
    no_init_pop1=10; no_init_pop2=10;
    gamePeriod=15;
    initPeriod=30;
    minPeriod=2;
    maxPeriod = 30;
    ratio1_2 = 1;
    count_pl1 =0;
    count_pl2 =0;
    recEvent1.fingerId=-1;
    recEvent1.type=-1;
    recEvent2.fingerId=-1;
    recEvent2.type=-1;
    ren = ren1;

    //time selection
    SDL_Texture* periodChoice;
    SDL_Texture* background;
    background = CSurface::OnLoad("../tilesets/farmsheet.png",ren);
    CArea::AreaControl.OnLoad("../maps/1.area", ren);
    char str[100];
    while (true)
    {
        CArea::AreaControl.OnRender(ren,-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
        sprintf(str,"Game period = %d min",gamePeriod);
        periodChoice = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 28,ren,400);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-3)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*8,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-1)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2+1)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2+2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*9,0,82,82);
        CSurface::OnDraw(periodChoice,ren,(MAP_WIDTH/2-2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE+TILE_SIZE/2-12);
        SDL_RenderPresent(ren);
        SDL_DestroyTexture(periodChoice);  periodChoice = NULL;
        SDL_Event Event;
        while(SDL_PollEvent(&Event)) {
            if(Event.type==SDL_KEYDOWN) {
                switch(Event.key.keysym.sym) {
                case SDLK_LEFT: {
                    gamePeriod -=1; 
                    if(gamePeriod<minPeriod) 
                        gamePeriod=maxPeriod;
                    break;
                                }
                case SDLK_RIGHT: {
                    gamePeriod+=1; 
                    if(gamePeriod>maxPeriod) 
                        gamePeriod=minPeriod;
                    break;
                                 }
                case SDLK_RETURN: {
                    goto end;							
                    break;
                                  }
                case SDLK_ESCAPE: {
                    Running =2;
                    goto end;	
                    break;
                                  }
                }
            }
            else if(Event.type==SDL_MOUSEBUTTONDOWN) {
                if(Event.button.button==SDL_BUTTON_LEFT) {
                    int i = (Event.button.y/CSurface::scaleY)/TILE_SIZE;
                    int j = (Event.button.x/CSurface::scaleX)/TILE_SIZE;
                    if(i==MAP_HEIGHT/2 && j==MAP_WIDTH/2-3) {
                        gamePeriod -=1; 
                        if(gamePeriod<minPeriod) 
                            gamePeriod=maxPeriod;
                    }
                    else if(i==MAP_HEIGHT/2 && j==MAP_WIDTH/2+2) {
                        gamePeriod+=1; 
                        if(gamePeriod>maxPeriod) 
                            gamePeriod=minPeriod;
                    }
                    else if(Event.button.x >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && Event.button.x <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0) {
                        Running=2;  
                        goto end;	
                    }
                    else 
                        return;
                }
            }
        }
    }
end:
    SDL_DestroyTexture(periodChoice);
    SDL_DestroyTexture(background);
    CArea::AreaControl.OnCleanup();
}

void CPlayersM::OnCleanup() {
    pl1.OnCleanup();	
    pl2.OnCleanup();
}