#include "CAppConstants.h"

int surround(int i, int j, int farm[FARM_HEIGHT][FARM_WIDTH])
{
    int count = 0;
    if(i<FARM_i || i>=FARM_HEIGHT+FARM_i || j<FARM_j || j>=FARM_WIDTH+FARM_j) 
        return -1;
    for(int m=i-1; m<=i+1; ++m) {
        for(int n=j-1; n<=j+1; ++n) {
            if(!(m<FARM_i || m>=FARM_HEIGHT+FARM_i || n<FARM_j || n>=FARM_WIDTH+FARM_j || m==n)) {
                if(farm[m-FARM_i][n-FARM_j]==1 || farm[m-FARM_i][n-FARM_j]==2) ++count;
            }
        }
    }
    return count;
}

//----------------------------------------------------------------------------------------
bool check8(int i, int j, int farm[FARM_HEIGHT][FARM_WIDTH], int choice)
{  
    for(int m=i-1; m<=i+1; ++m) {
        for(int n=j-1; n<=j+1; ++n) {
            if(m==n && choice==-1) 
                continue;
            if(surround(m,n,farm)>=4 && (farm[m-FARM_i][n-FARM_j]==1||farm[m-FARM_i][n-FARM_j]==2)) 
                return false;
        }
    }
    return true;
}

/*void logSDLError(FILE* os, char msg[]){
    //os<<msg<<" error: "<<SDL_GetError()<<std::endl;
    fprintf(os,"\nerror: %s\n",msg);		
}*/

void wait_key_click()
{
    SDL_Event e;
    while(true) {
        while(SDL_PollEvent(&e)) {
            if(e.type==SDL_KEYDOWN || e.type==SDL_MOUSEBUTTONDOWN) return;
        }
    }
}

void message(SDL_Renderer* ren, char str[], int x, int y,int wraplen)
{
    SDL_Texture* tempTex;
    if(wraplen)
        tempTex = CSurface::OnLoadText(str, "../SourceSansPro-Regular.ttf", 255,255,255, 32,ren,wraplen);
    else tempTex = CSurface::OnLoadText(str, "../SourceSansPro-Regular.ttf", 255,255,255, 32,ren);
    CSurface::OnDraw(tempTex,ren,x,y);
    SDL_RenderPresent(ren);
    wait_key_click();
    SDL_DestroyTexture(tempTex);
}