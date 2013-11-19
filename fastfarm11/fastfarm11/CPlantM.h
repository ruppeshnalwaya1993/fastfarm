#ifndef _CPLANTM_H_
    #define _CPLANTM_H_
#include "CPlant.h"
#include "CPlayersM.h"

enum{
  NONE=0, NORMAL1, NORMAL2, UNDER1, UNDER2, OVER1, OVER2, DYING1, DYING2
};

class CPlantM : public CPlant{
    protected:
		static SDL_Texture*    tex;
 
    public:
        CPlantM();
 
        //~CPlant();
 
    public:
        bool OnLoad(char* File, int Width, int Height, int MaxFrames, SDL_Renderer *ren, int scaleW = -1, int scaleH=-1);
 
        static void OnLoopStatic();
 
        void OnRender(SDL_Renderer* ren);
 
        static void OnCleanupStatic();
		void OnEvent(SDL_Event* Event);
		void OnLoop();
};

#endif