//==============================================================================
#include "CApp.h"

//--------------------------------------------------------------------------------
void levelSel(int& level, SDL_Renderer* ren, int& Running)
{
    SDL_Texture* levelChoice;
    SDL_Texture* background;
    int range;
    FILE* fp=fopen("../maps/index.level","r");
    if(fp==NULL) return;
    fscanf(fp,"%d",&range);
    fclose(fp);

    background = CSurface::OnLoad("../tilesets/farmsheet.png",ren);
    char str[50];
    while (true)
    {
        CArea::AreaControl.OnRender(ren,-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
        sprintf(str,"Puzzle No. = %d",level);
        levelChoice = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 28,ren,400);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-3)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*8,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2-1)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2+1)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*4,0,82,82);
        CSurface::OnDraw(background,ren,(MAP_WIDTH/2+2)*TILE_SIZE,(MAP_HEIGHT/2)*TILE_SIZE,82*9,0,82,82);
        CSurface::OnDraw(levelChoice,ren,(MAP_WIDTH/2-2)*TILE_SIZE+TILE_SIZE/2,(MAP_HEIGHT/2)*TILE_SIZE+TILE_SIZE/2-12);
        SDL_RenderPresent(ren);
        SDL_DestroyTexture(levelChoice);  levelChoice = NULL;
        SDL_Event Event;
        while(SDL_PollEvent(&Event)) {
            if(Event.type==SDL_KEYDOWN)
            {	switch(Event.key.keysym.sym)
            {
            case SDLK_LEFT: {level = 1+(level-2+range)%range;
                break;}
            case SDLK_RIGHT: {level = 1+level%range;
                break;}
            case SDLK_RETURN: { goto endsel;							
                break;}
            case SDLK_ESCAPE: {Running=2;
                goto endsel;
                break;}
            }
            }
            else if(Event.type==SDL_MOUSEBUTTONDOWN)
            {
                if(Event.button.button==SDL_BUTTON_LEFT)
                {
                    int i = (Event.button.y/CSurface::scaleY)/TILE_SIZE;
                    int j = (Event.button.x/CSurface::scaleX)/TILE_SIZE;
                    if(i==MAP_HEIGHT/2 && j==MAP_WIDTH/2-3)
                        level = 1+(level-2+range)%range;
                    else if(i==MAP_HEIGHT/2 && j==MAP_WIDTH/2+2)
                        level = 1+level%range;
                    else if(i==MAP_HEIGHT/2 && j>=MAP_WIDTH/2-2 && j<MAP_WIDTH/2+2)
                        goto endsel;
                    else if(Event.button.x >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && Event.button.x <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
                    {Running=2;  goto endsel;}
                }
            }
        }
    }
endsel:
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(levelChoice);
}
//----------------------------------------------------------------------------------------
bool check(int i, int j)
{
    for(int m=i-2; m<=i+2; ++m)
        for(int n=j-2; n<=j+2; ++n)
            if(surround(m,n,CPlant::farm)>=4) return false;
    return true;
}
//----------------------------------------------------------------------------------------
bool fillNcheck(int i, int j, int p)
{
    CPlant::farm[i][j]=1;  int a,limit =0;
    vector< pair<int,int> > coords;
    for(limit=0,a=0; a<p && limit<10; ++a, ++limit)
    {
        int y = i-1+rand()%3; int x = j-1+rand()%3;
        if(CPlant::farm[y][x]==1||CPlant::farm[y][x]==2) 
        { --a;  continue;}
        CPlant::farm[y][x]=1;
        pair<int,int> pp(y,x);
        coords.push_back(pp);
    }
    if(check(i,j)) return true;
    for(a =0; a<coords.size(); ++a)
        CPlant::farm[coords[a].first][coords[a].second]=0;
    return false;
}

//----------------------------------------------------------------------------------
bool loadWarehouse(SDL_Renderer* ren)
{  //load seedling 
    CEntity* ent = new CSeedlingW;
    if(ent->OnLoad("../tilesets/farmsheet.png", 82, 82, 1, ren)  == false) {
        return false;}
    ent->Y = 0;
    ent->X = TILE_SIZE*(3);
    ent->AnimState = 11;
    CEntity::EntityList.push_back(ent);
}

//----------------------------------------------------------------------------------
bool loadRandomStable(SDL_Renderer* ren, int N)
{
    int i,j; int limit = 500;
    while(N && limit--) {
        i=rand()%MAP_HEIGHT;  
        j=rand()%(MAP_WIDTH-4);

        if(CPlant::farm[i][j] == 0) {
            if( surround(i,j,CPlant::farm)==2 || surround(i,j,CPlant::farm)==3 ) {
                if(surround(i-1,j-1,CPlant::farm)!=3 && surround(i-1,j,CPlant::farm)!=3 && surround(i-1,j+1,CPlant::farm)!=3 && surround(i,j-1,CPlant::farm)!=3 && surround(i,j+1,CPlant::farm)!=3 && surround(i+1,j-1,CPlant::farm)!=3 && surround(i+1,j,CPlant::farm)!=3 && surround(i+1,j+1,CPlant::farm)!=3) {
                    CPlant::farm[i][j]=1;  --N;
                }
            }
            else {
                if(surround(i,j,CPlant::farm)<2) {
                    int range = 3-surround(i,j,CPlant::farm); 
                    int count = range;
                    int p = 1+rand()%range;
                    while(count--) {
                        //fill and check
                        if(fillNcheck(i,j,p)) {  
                            --N;  
                            break;
                        }
                        else { 
                            p = 1+(p+1)%range;
                        }
                    }
                }
            }
        }
    }
    return true;
}

//----------------------------------------------------------------------------------
bool loadStable(SDL_Renderer* ren, char* filename)
{
    FILE* fp = fopen(filename,"r");
    if(fp == NULL) printf("not farming");
    int i=0,j=0; char ch;
    while(fp && i<FARM_HEIGHT) {
        ch = fgetc(fp);
        if(feof(fp)) break;
        if(ch == '\n') {
            ++i; 
            j=0; 
            continue;
        }

        CPlant::farm[i][j] = ch-48;
        ++j;
    }
    fclose(fp);


    for (i = FARM_i; i < FARM_HEIGHT+FARM_i; ++i) { 
        for(j=FARM_j; j<FARM_WIDTH+FARM_j; ++j) {  
            if(CPlant::farm[i-FARM_i][j-FARM_j] == 0) {
                printf("0"); continue;
            }
            if(CPlant::farm[i-FARM_i][j-FARM_j] == 3) {
                printf("3"); continue;
            }
            printf("%d",CPlant::farm[i-FARM_i][j-FARM_j]);
            CEntity* ent = new CPlant;
            if(ent->OnLoad("../tilesets/weedsheet.png", 82, 82, 8, ren)  == 0) {  
                printf("\nnot farming 2\n");
                return false;
            }

            ent->Y = i*TILE_SIZE+TILE_SIZE/2 - ent->getCenter().second;
            ent->X = j*TILE_SIZE+TILE_SIZE/2 - ent->getCenter().first;
            CEntity::EntityList.push_back(ent);
        }
        printf("\n");
    }
    return true;
}

//==============================================================================
bool CApp::OnInit() {

    CSeedlingW::no_seedling = 0;
    CSeedlingW::seedling_purch = 0;
    CSeedlingW::moves = 0;

    try {
        char str[50];
        sprintf(str,"Moves = %d \nPurchases = %d \nStock = %d",CSeedlingW::moves,CSeedlingW::seedling_purch,CSeedlingW::no_seedling);
        textTex = CSurface::OnLoadText(str,"../CooperBlackStd.otf", 110,236,4, 22,ren,600);
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return false;
    }

    if(CArea::AreaControl.OnLoad("../maps/1.area", ren) == false) {
        return false;
    }
    loadWarehouse(ren);
    char str[50];

    //level selector
    levelSel(level,ren,Running);

    sprintf(str,"../maps/%d.level",level);
    loadStable(ren, str);

    //crop
    crop = CSurface::OnLoad("../tilesets/seedling.png",ren);
    if(crop== NULL) return false; 
    return true;
}

//==============================================================================
