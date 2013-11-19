#include "CPlantM.h"

SDL_Texture* CPlantM::tex = NULL;

CPlantM::CPlantM()
{
	Anim_Control.wait_time = 5000;
	Anim_Control.Oscillate = false;
}

//------------------------------------------------------------------------------------------------
void CPlantM::OnEvent(SDL_Event* Event)
{
}

//-------------------------------------------------------------------------------------------------
bool CPlantM::OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW, int scaleH)
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
void CPlantM::OnRender(SDL_Renderer* ren)
{
    if(tex == NULL || ren == NULL) return;
 
	CSurface::OnDraw(tex, ren, X, Y, AnimState * Width, Anim_Control.GetCurrentFrame() * Height, Width, Height, scaleW, scaleH);
}

//--------------------------------------------------------------------------------------------------
void CPlantM::OnCleanupStatic() {
    if(tex) {
		SDL_DestroyTexture(tex);
    }
 
    tex = NULL;
}

//---------------------------------------------------------------------------------------------------
void CPlantM::OnLoopStatic()
{
	float r1=0,r2=0;
	vector<pair<int,int>> todie;
	pair<int,int> p;
	for(int i = FARM_i; i<FARM_HEIGHT+FARM_i; ++i)
	{
		for(int j =FARM_j; j<FARM_WIDTH+FARM_j; ++j)
		{
			int count1 = 0, count2=0;
			if(farm[i-FARM_i][j-FARM_j] == NONE ) continue;
			
			if(i-FARM_i>0 && j-FARM_j>0)						{	 if(farm[i-FARM_i-1][j-FARM_j-1]!=0  && farm[i-FARM_i-1][j-FARM_j-1]&1)	++count1;
																else if(farm[i-FARM_i-1][j-FARM_j-1]!=0) count2++;} 	
			if(i-FARM_i>0)										{	 if(farm[i-FARM_i-1][j-FARM_j]!=0	&& farm[i-FARM_i-1][j-FARM_j]&1)	++count1;
																else if(farm[i-FARM_i-1][j-FARM_j]!=0)	++count2;}
			if(i-FARM_i>0 && j-FARM_j<FARM_WIDTH-1)				{	 if(farm[i-FARM_i-1][j-FARM_j+1]!=0	&& farm[i-FARM_i-1][j-FARM_j+1]&1) ++count1; 
																else if(farm[i-FARM_i-1][j-FARM_j+1]!=0) ++count2;}
			if(j-FARM_j>0)										{	 if(farm[i-FARM_i][j-FARM_j-1]!=0	&& farm[i-FARM_i][j-FARM_j-1]&1)	++count1; 
																else if(farm[i-FARM_i][j-FARM_j-1]!=0) ++count2;}
			if(j-FARM_j<FARM_WIDTH-1)							{	 if(farm[i-FARM_i][j-FARM_j+1]!=0	&& farm[i-FARM_i][j-FARM_j+1]&1)	++count1; 
																else if(farm[i-FARM_i][j-FARM_j+1]!=0) ++count2;}
			if(i-FARM_i<FARM_HEIGHT-1 && j-FARM_j>0)			{	 if(farm[i-FARM_i+1][j-FARM_j-1]!=0	&& farm[i-FARM_i+1][j-FARM_j-1]&1) ++count1;  
																else if(farm[i-FARM_i+1][j-FARM_j-1]!=0) ++count2;}
			if(i-FARM_i<FARM_HEIGHT-1)							{	 if(farm[i-FARM_i+1][j-FARM_j]!=0	&& farm[i-FARM_i+1][j-FARM_j]&1)	++count1; 
																else if(farm[i-FARM_i+1][j-FARM_j]!=0) ++count2;}
			if(i-FARM_i<FARM_HEIGHT-1 && j-FARM_j<FARM_WIDTH-1)	{if(farm[i-FARM_i+1][j-FARM_j+1]!=0	&& farm[i-FARM_i+1][j-FARM_j+1]&1) ++count1; 
																else if(farm[i-FARM_i+1][j-FARM_j+1]!=0) ++count2;}

			p.first = i; p.second = j;  //if(i==10 && j==10) printf("%d",count);
			if(farm[i-FARM_i][j-FARM_j]&1){
				++r1;
				if((count1<2 && farm[i-FARM_i][j-FARM_j]!=UNDER1) || (count1>4 && farm[i-FARM_i][j-FARM_j]!=OVER1)) {
					todie.push_back(p);}
				else if(count1<count2) todie.push_back(p);
			}
			else {
				++r2;
				if((count2<2 && farm[i-FARM_i][j-FARM_j]!=UNDER2) || (count2>4 && farm[i-FARM_i][j-FARM_j]!=OVER2)) {
					todie.push_back(p);}
				else if(count2<count1) todie.push_back(p);
			}

		}
	}

	CPlayersM::playerControl.ratio1_2 = (r1+1)/(r2+1);
	CPlayersM::playerControl.count_pl1 = r1;
	CPlayersM::playerControl.count_pl2 = r2;
	for(int i=0; i < todie.size(); ++i)
	{
		farm[todie[i].first-FARM_i][todie[i].second-FARM_j] = 0;
	}
}

//--------------------------------------------------------------- 
void CPlantM::OnLoop()
{
	CPlant::OnLoop();
}