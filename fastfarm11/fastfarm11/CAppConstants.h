#ifndef _CAPPCONSTANSTS_H_
#define _CAPPCONSTANSTS_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <SDL.h>
#include "CSurface.h"

#define MAP_WIDTH    24
#define MAP_HEIGHT   14
#define FARM_WIDTH 24
#define FARM_HEIGHT 13
#define FARM_i 1
#define FARM_j 0

using namespace std;

const int SCREEN_WIDTH  = 1968;  //1920 //1366
const int SCREEN_HEIGHT = 1148;  //1080 //768
//We'll just be using square tiles for now
const int TILE_SIZE = 82;


//-----------------------------------------------------------------------------------
int surround(int i, int j, int farm[FARM_HEIGHT][FARM_WIDTH]);
bool check8(int i, int j, int farm[FARM_HEIGHT][FARM_WIDTH], int choice=0);
void wait_key_click();
void message(SDL_Renderer* ren, char str[], int x, int y,int wraplen=0);
#endif