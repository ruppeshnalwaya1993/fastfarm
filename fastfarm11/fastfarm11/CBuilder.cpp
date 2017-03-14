//==============================================================================
#include "CBuilder.h"
//==============================================================================
CBuilder::CBuilder(SDL_Renderer* ren1, SDL_Window* win1) {
    win = win1;
    ren = ren1;
    Running = 1;
    saveText = nullptr;
    saveB = nullptr;
    build_level_text = nullptr;
    crop = NULL;
    level=0;
    plant.deadstate = 0;
}

//------------------------------------------------------------------------------
int CBuilder::OnExecute() {
    Running = 1;
    if(OnInit() == false) {
        return -1;
    }

    SDL_Event Event;
    while(Running==1) {
        while(SDL_PollEvent(&Event)) {
            OnEvent(&Event);
        }

        OnLoop();
        OnRender();
    }

    OnCleanup();

    return Running;
}

bool CBuilder::OnInit(){
    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++) {
        for (int j = FARM_j; j < FARM_WIDTH+FARM_j; j++) {
            farm[i-FARM_i][j-FARM_j]=0;
        }
    }
    if(plant.OnLoad("../tilesets/weedsheet.png", 82, 82, 8, ren)  == false) {
        return false;
    }
    FILE* fp = fopen("../maps/index.level","r");
    if(fp==NULL) {
        printf("error reading index"); 
        Running=2;  
        return false;
    }
    fscanf(fp,"%d",&level);
    fclose(fp);
    ++level;

    if(CArea::AreaControl.OnLoad("../maps/1.area", ren) == false) {
        return false;
    }
    saveB = CSurface::OnLoad("../tilesets/farmsheet.png",ren);
    if(saveB==NULL) 
        return false;
    try {
        saveText = CSurface::OnLoadText("Save", "../CooperBlackStd.otf", 110,236,4, 28,ren,200);
        char str[200];
        sprintf(str,"Building Level %d",level);
        build_level_text = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 28,ren,300);
    }
    catch (const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        return false;
    }
    //crop
    crop = CSurface::OnLoad("../tilesets/seedling.png",ren);
    if(crop== NULL) return false; 
    return true;
}

void CBuilder::OnLoop() {
    plant.OnLoop();
    char str[200];
    SDL_DestroyTexture(build_level_text); 
    build_level_text = NULL;
    sprintf(str,"Building Level %d",level);
    build_level_text = CSurface::OnLoadText(str, "../CooperBlackStd.otf", 110,236,4, 28,ren,300);
}

void CBuilder::OnRender() {
    CArea::AreaControl.OnRender(ren,-CCamera::CameraControl.GetX(), -CCamera::CameraControl.GetY());
    CSurface::OnDraw(saveB,ren,(1)*TILE_SIZE,0,10*TILE_SIZE,0,82,82);
    CSurface::OnDraw(saveText,ren,(1)*TILE_SIZE+5,TILE_SIZE/2-10);
    CSurface::OnDraw(build_level_text,ren,(MAP_WIDTH/2-1)*TILE_SIZE - TILE_SIZE/2,(0.5)*TILE_SIZE);
    for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++) {
        for (int j = 0; j < FARM_WIDTH+FARM_j; j++) {
            if(farm[i-FARM_i][j-FARM_j]==1 || farm[i-FARM_i][j-FARM_j]==2) {
                plant.X = j*TILE_SIZE+TILE_SIZE/2 - plant.getCenter().first;  
                plant.Y = i*TILE_SIZE+TILE_SIZE/2 - plant.getCenter().second;
                plant.OnRender(ren);
            }
            else if(farm[i-FARM_i][j-FARM_j]==3) {
                CSurface::OnDraw(crop,ren,j*TILE_SIZE,i*TILE_SIZE);
            }
        }
    }
    SDL_RenderPresent(ren);
}

void CBuilder::OnCleanup() {
    CArea::AreaControl.OnCleanup();
    SDL_DestroyTexture(saveB);  saveB=NULL;
    SDL_DestroyTexture(saveText); saveText=NULL;
    SDL_DestroyTexture(build_level_text); build_level_text=NULL;
    SDL_DestroyTexture(crop);  crop =NULL;
    plant.OnCleanup();
    CPlant::OnCleanupStatic();
}
void CBuilder::OnExit() {
    Running = 0;
}
void CBuilder::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat) {
    switch(sym) {
    case SDLK_UP:    CCamera::CameraControl.OnMove( 0,-5); break;
    case SDLK_DOWN:  CCamera::CameraControl.OnMove( 0, 5); break;
    case SDLK_LEFT:  CCamera::CameraControl.OnMove(-5, 0); break;
    case SDLK_RIGHT: CCamera::CameraControl.OnMove( 5, 0); break;
    case SDLK_ESCAPE: Running = 2;

    default: {
             }
    }
}
void CBuilder::OnLButtonDown(int mX, int mY) {

    int j=(mX/CSurface::scaleX)/TILE_SIZE;
    int i=(mY/CSurface::scaleY)/TILE_SIZE;
    printf("%d %d %d\n",i,j,surround(i,j,farm));
    if(i>=FARM_i && i<FARM_HEIGHT+FARM_i && j>=FARM_j && j<FARM_WIDTH+FARM_j) {
        if(farm[i-FARM_i][j-FARM_j]==1 || farm[i-FARM_i][j-FARM_j]==2) {
            farm[i-FARM_i][j-FARM_j]=3;
        }
        else if(farm[i-FARM_i][j-FARM_j]==3) {
            farm[i-FARM_i][j-FARM_j]=0;
        }
        else if(surround(i,j,farm)<4 && check8(i,j,farm,-1)) {
            farm[i-FARM_i][j-FARM_j]=1;
        }
    }
    else if(j==1 && i==0) {
        char str[50];
        sprintf(str,"../maps/%d.level",level);
        FILE* fp = fopen(str,"w");
        if(fp==NULL) {
            printf("error saving level"); 
            Running=2;  
            return;
        }
        for (int i = FARM_i; i < FARM_HEIGHT+FARM_i; i++) {
            for (int j = FARM_j; j < FARM_WIDTH+FARM_j; j++) {
                fprintf(fp,"%d",farm[i-FARM_i][j-FARM_j]);
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
        fp = fopen("../maps/index.level","w");
        if(fp==NULL) {
            printf("error writing index"); Running=2;  return ;
        }
        fprintf(fp,"%d",level);
        fclose(fp);
        ++level;
        message(ren,"Saved",(MAP_WIDTH/2-1)*TILE_SIZE,(MAP_HEIGHT/2-1)*TILE_SIZE);
    }
    else if(mX >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && mX <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
        Running=2;

}

void CBuilder::OnEvent(SDL_Event* Event) {
    CEvent::OnEvent(Event);
}