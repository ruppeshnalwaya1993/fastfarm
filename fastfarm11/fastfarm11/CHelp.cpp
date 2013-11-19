//==============================================================================
#include "CHelp.h"
//==============================================================================
CHelp::CHelp(SDL_Renderer* ren1, SDL_Window* win1) {
	win = win1;
	ren = ren1;
    Running = 1;
	index=0;
	no_pages=0;
}

//------------------------------------------------------------------------------
int CHelp::OnExecute(char str[]) {
	strcpy(helpfile,str);
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

bool CHelp::OnInit(){
	index=0;
	no_pages=0;
	
	FILE* fp = fopen(helpfile,"r");
	  if(fp==NULL) {printf("error reading help"); Running=2;  return false;}
	  fscanf(fp,"%d",&no_pages);
	  fgetc(fp);
	  for (int i = 0; i < no_pages; i++)
	  {
		  char str[200];
		  fscanf(fp,"%s",str);
		  SDL_Texture* temptex = CSurface::OnLoad(str,ren);
		  pages.push_back(temptex);
	  }
	
	  fclose(fp);
	return true;
}

void CHelp::OnLoop(){
}

void CHelp::OnRender(){
	if(index<no_pages)
	CSurface::OnDraw(pages[index],ren,0,0);
	else Running=2;
	SDL_RenderPresent(ren);
}

void CHelp::OnCleanup(){
	for (int i = 0; i < no_pages; i++)
	{
		SDL_DestroyTexture(pages[i]);
	}
	pages.clear();
}
void CHelp::OnExit(){
Running = 0;
}
void CHelp::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat){
  switch(sym) {
        case SDLK_UP:    CCamera::CameraControl.OnMove( 0,-5); break;
		case SDLK_DOWN:  CCamera::CameraControl.OnMove( 0, 5); break;
        case SDLK_LEFT:  CCamera::CameraControl.OnMove(-5, 0); break;
        case SDLK_RIGHT: CCamera::CameraControl.OnMove( 5, 0); break;
		case SDLK_ESCAPE: Running = 2;
 
		default: {index++;
        }
    }
}
void CHelp::OnLButtonDown(int mX, int mY){
	int j=(mX/CSurface::scaleX)/TILE_SIZE;
	int i=(mY/CSurface::scaleY)/TILE_SIZE;

	if(mX >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && mX <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
		Running=2;
	else index++;

}
void CHelp::OnEvent(SDL_Event* Event){
CEvent::OnEvent(Event);
}