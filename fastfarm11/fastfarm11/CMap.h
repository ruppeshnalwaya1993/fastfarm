#ifndef _CMAP_H_
#define _CMAP_H_

#include <SDL.h>
#include <vector>

#include "CTile.h"
#include "CSurface.h"

class CMap {
public:
    SDL_Texture* tex_Tileset;

private:
    std::vector<CTile> TileList;

public:
    CMap();

public:
    bool OnLoad(char* File);

    void OnRender(SDL_Renderer* ren, int MapX, int MapY); 
    CTile*    GetTile(int X, int Y);
};

#endif