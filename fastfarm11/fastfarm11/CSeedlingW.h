#ifndef _CSEEDLINGW_H_
    #define _CSEEDLINGW_H_
#include "CPlant.h"

class CSeedlingW : public CEntity
{
private:
	CPlant* plantC;
	bool clickHold;
	SDL_Renderer* ren;

public:
	CSeedlingW();
	
	static int no_seedling;
	static int seedling_purch;
	static int moves;

	void OnEvent(SDL_Event* Event);
	void OnRender(SDL_Renderer* ren);
	void OnLoop();
	void OnCleanup();
};

#endif