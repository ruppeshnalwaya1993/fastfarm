//==============================================================================
// Surface functions
//==============================================================================
#ifndef _CSURFACE_H_
    #define _CSURFACE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "CAppConstants.h"

//==============================================================================
class CSurface {
	public:
		CSurface();
		
		static float scaleX;
		static float scaleY;

	public:
		static SDL_Texture* OnLoad(char file[], SDL_Renderer *ren);

		static SDL_Texture* OnLoadText(std::string message, std::string fontFile, int R, int G, int B, int fontSize, SDL_Renderer *ren, Uint32 len=0);

		static bool OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);

		static bool OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

		static bool OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int x2, int y2, int w2, int h2, int w=-1, int h=-1);
};

//==============================================================================

#endif