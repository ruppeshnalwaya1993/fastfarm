#include <stdio.h>
#include <SDL.h>
#include <iostream>
#include <string>
#include "CApp.h"
#include "CMenu.h"
#include "CBuilder.h"
#include "CHelp.h"
#include "CMulti.h"

using namespace std;

int main(int argc, char **argv){
	SDL_Renderer* ren; SDL_Window* window;
	 if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
	//logSDLError(stdout, "SDL_Init");
		printf("error3");
		return false;
	}

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG){
		printf("img png error\n");
		return false;
	}

	if (TTF_Init() == -1){
    std::cout << TTF_GetError() << std::endl;
    return false;
	}

	//screen properties
	SDL_DisplayMode current;
	for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){
    int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
    if(should_be_zero != 0)
      // In case of error...
      printf("Could not get display mode for video display #%d: %s", i, SDL_GetError());
    else 
      {// On success, print the current display mode.
      printf("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);
	  
	  CSurface::scaleY = (float)current.h/SCREEN_HEIGHT;
	  CSurface::scaleX = (float)current.w/SCREEN_WIDTH;
	  printf("scalar:%lf %lf\n",CSurface::scaleX,CSurface::scaleY);
	  }
	}

	window = SDL_CreateWindow("Lesson 2", 10, 10, SCREEN_WIDTH*CSurface::scaleX, SCREEN_HEIGHT*CSurface::scaleY, SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN);
	if (window == nullptr){
//	logSDLError(std::cout, "CreateWindow");
		printf("error4");
	return false;
	}
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
//	logSDLError(std::cout, "CreateRenderer");
		printf("error5");
	return false;
	}

	//scaling
	SDL_RenderSetScale(ren,CSurface::scaleX,CSurface::scaleY);

	//debug
	

	CMenu theMenu(ren,window);
	CApp theApp(ren,window);
	CBuilder theBuilder(ren,window);
	CHelp theHelp(ren,window);
	CMulti theMulti(ren,window);
	while (true)
	{
	int ret = theMenu.OnExecute();
	if(ret==2){if(theApp.OnExecute()==0) break;}
	else if(ret==3) {if(theMulti.OnExecute()==0) break;}
	else if(ret==4){if(theBuilder.OnExecute()==0) break;}
	else if(ret==5) {if(theHelp.OnExecute("../maps/instr_pzzC.help")==0) break;}
	else if(ret==6) {if(theHelp.OnExecute("../maps/instr2p.help")==0) break;}
	else if(ret==7) {if(theHelp.OnExecute("../maps/instr_builder.help")==0) break;}
		else break;
	}  

	CArea::AreaControl.OnCleanup();
	CPlant::OnCleanupStatic();
	SDL_DestroyRenderer(ren); 
	SDL_DestroyWindow(window);
	

	IMG_Quit();
	SDL_Quit();
    //return theApp.OnExecute();

	return 0;
}