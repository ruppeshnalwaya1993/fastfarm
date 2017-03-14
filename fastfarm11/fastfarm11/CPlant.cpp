#include "CPlant.h"

SDL_Texture* CPlant::tex = NULL;
int CPlant::farm[FARM_HEIGHT][FARM_WIDTH] = {0};

CPlant::CPlant()
{
    clickHold = false;
    Anim_Control.Oscillate = true;
    Anim_Control.wait_time = 1000;
    deadstate = 2;
}

//------------------------------------------------------------------------------------------------
void CPlant::OnEvent(SDL_Event* Event)
{/*
    switch(Event->type)
    {
    case SDL_MOUSEBUTTONDOWN: {
        switch(Event->button.button) {
        case SDL_BUTTON_LEFT: {
            if((Event->button.x)/CSurface::scaleX >= X && (Event->button.y)/CSurface::scaleY >= Y && (Event->button.x)/CSurface::scaleX < X+Width && (Event->button.y)/CSurface::scaleY < Y+Height) {
                clickHold = true;
                X=(Event->button.x)/CSurface::scaleX-getCenter().first; Y=(Event->button.y)/CSurface::scaleY-getCenter().second;
            }
            break;
                              }
        }
        break;
                              }

    case SDL_MOUSEBUTTONUP:    {
        switch(Event->button.button) {
        case SDL_BUTTON_LEFT: {
            clickHold = false;
            break;
                              }
        }
        break;
                               }

    case SDL_MOUSEMOTION: {
        if(clickHold) {
            X=(Event->button.x)/CSurface::scaleX-getCenter().first; Y=(Event->button.y)/CSurface::scaleY-getCenter().second;
        }
        break;
                          }
    }*/
}

//-------------------------------------------------------------------------------------------------
bool CPlant::OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW, int scaleH)
{
    if(tex ==NULL) 
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

//------------------------------------------------------------------------------------------------
void CPlant::OnRender(SDL_Renderer* ren)
{
    if(tex == NULL || ren == NULL) 
        return;

    CSurface::OnDraw(tex, ren, X, Y, AnimState * Width, Anim_Control.GetCurrentFrame() * Height, Width, Height, scaleW, scaleH);
}

//--------------------------------------------------------------------------------------------------
void CPlant::OnCleanupStatic() {
    if(tex) {
        SDL_DestroyTexture(tex);
    }

    tex = NULL;
}

//---------------------------------------------------------------------------------------------------
int CPlant::OnLoopStatic()
{
    int reprod=0;
    vector<pair<int,int>> todie;
    pair<int,int> p;
    for(int i = FARM_i; i<FARM_HEIGHT+FARM_i; ++i)
    {
        for(int j =FARM_j; j<FARM_WIDTH+FARM_j; ++j)
        {
            int count = 0;
            if(farm[i-FARM_i][j-FARM_j] == 0 || farm[i-FARM_i][j-FARM_j] == 3) 
                continue;

            if(i-FARM_i>0 && j-FARM_j>0)						if(farm[i-FARM_i-1][j-FARM_j-1]==1	|| farm[i-FARM_i-1][j-FARM_j-1]==2)	++count; 
            if(i-FARM_i>0)										if(farm[i-FARM_i-1][j-FARM_j]==1	|| farm[i-FARM_i-1][j-FARM_j]==2)	++count;
            if(i-FARM_i>0 && j-FARM_j<FARM_WIDTH-1)				if(farm[i-FARM_i-1][j-FARM_j+1]==1	|| farm[i-FARM_i-1][j-FARM_j+1]==2) ++count;
            if(j-FARM_j>0)										if(farm[i-FARM_i][j-FARM_j-1]==1	|| farm[i-FARM_i][j-FARM_j-1]==2)	++count;
            if(j-FARM_j<FARM_WIDTH-1)							if(farm[i-FARM_i][j-FARM_j+1]==1	|| farm[i-FARM_i][j-FARM_j+1]==2)	++count;
            if(i-FARM_i<FARM_HEIGHT-1 && j-FARM_j>0)			if(farm[i-FARM_i+1][j-FARM_j-1]==1	|| farm[i-FARM_i+1][j-FARM_j-1]==2) ++count;
            if(i-FARM_i<FARM_HEIGHT-1)							if(farm[i-FARM_i+1][j-FARM_j]==1	|| farm[i-FARM_i+1][j-FARM_j]==2)	++count;
            if(i-FARM_i<FARM_HEIGHT-1 && j-FARM_j<FARM_WIDTH-1)	if(farm[i-FARM_i+1][j-FARM_j+1]==1	|| farm[i-FARM_i+1][j-FARM_j+1]==2) ++count;

            p.first = i; 
            p.second = j;  //if(i==10 && j==10) printf("%d",count);
            if(count<2 || count>4) todie.push_back(p);
            if(count == 4 && farm[i-FARM_i][j-FARM_j]==1) {
                ++reprod;  
                farm[i-FARM_i][j-FARM_j]=2;
            }
        }
    }

    for(int i=0; i < todie.size(); ++i)
    {
        farm[todie[i].first-FARM_i][todie[i].second-FARM_j] = 0;
    }
    return reprod;
}

//--------------------------------------------------------------- 
void CPlant::OnLoop()
{
    CEntity::OnLoop();
    int i,j;
    j= (X+getCenter().first)/TILE_SIZE;
    i= (Y+getCenter().second)/TILE_SIZE;
    if(farm[i-FARM_i][j-FARM_j]==0) {
        alive = false;  
        //printf("%d %d\n",i,j);
    }
    if(farm[i-FARM_i][j-FARM_j]== 2) 
        AnimState =1;
}