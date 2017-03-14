#ifndef _CAREA_H_
#define _CAREA_H_

#include "CMap.h"

class CArea {
public:
    static CArea            AreaControl;

public:
    std::vector<CMap>       MapList;

private:
    int                     AreaSize;

    SDL_Texture*        tex_Tileset;

public:
    CArea();

    bool    OnLoad(char* File, SDL_Renderer* ren);

    void    OnRender(SDL_Renderer* ren, int CameraX, int CameraY);

    void    OnCleanup();
    CMap*   GetMap(int X, int Y);
    CTile*  GetTile(int X, int Y);
};

#endif