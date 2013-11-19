//==============================================================================
#include "CSurface.h"
#include <stdio.h>

//==============================================================================
CSurface::CSurface() {
}

float CSurface::scaleX = 1.0;
float CSurface::scaleY = 1.0;

//==============================================================================
SDL_Texture* CSurface::OnLoad(char file[], SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file);
	if (texture == nullptr)		
		//logSDLError(std::cout, "LoadTexture");
			printf("error load texture");
	return texture;
}

//==============================================================================
SDL_Texture* CSurface::OnLoadText(std::string message, std::string fontFile, int R, int G, int B, int fontSize, SDL_Renderer *ren, Uint32 len)
{
    //Open the font
	TTF_Font *font = nullptr;
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr)
	{   throw std::runtime_error("Failed to load font: " + fontFile + TTF_GetError());
		return nullptr;
	}
	
	//make color
	SDL_Color color;
	color.r=R; color.g=G; color.b=B;

    //Render the message to an SDL_Surface, as that's what TTF_RenderText_X returns
	SDL_Surface *surf;
	if(len>0)
		surf = TTF_RenderText_Blended_Wrapped(font, message.c_str(), color,len);
	else
		surf = TTF_RenderText_Blended(font, message.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surf);
    //Clean up unneeded stuff
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
 
    return texture;
}

//==============================================================================
bool CSurface::OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(ren, tex, NULL, &dst);

	return true;
}

//------------------------------------------------------------------------------
bool CSurface::OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h){
	if(tex == NULL || ren == NULL) {
		return false;
	}

	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);

	return true;
}

//==============================================================================
bool CSurface::OnDraw(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int x2, int y2, int w2, int h2, int w, int h){
	if(tex == NULL || ren == NULL) {
		return false;
	}
	
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	/*if(w==-1 || h==-1)
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);*/

	dst.x = x;
	dst.y = y;
	if(w!=-1)
	dst.w = w;
	else dst.w = w2;
	if(h!=-1)
	dst.h = h;
	else dst.h = h2;

	SDL_Rect src;
	src.x = x2;
	src.y = y2;
	src.w = w2;
	src.h = h2;

	SDL_RenderCopy(ren, tex, &src, &dst);

	return true;
}

//==============================================================================