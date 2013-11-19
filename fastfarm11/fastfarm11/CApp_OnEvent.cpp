//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnEvent(SDL_Event* Event) {
/*    if(Event->type == SDL_QUIT) {
        Running = false;
    }
*/
	CEvent::OnEvent(Event);

	for(int i=0; i<CEntity::EntityList.size(); ++i)
	{
		CEntity::EntityList[i]->OnEvent(Event);
	}
}

void CApp::OnExit() {
    Running = 2;
}

void CApp::OnKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode, Uint8 repeat) {
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

void CApp::OnLButtonDown(int mX, int mY) {
    printf("mouse: %d %d\n", mX,mY);
	int i=(mY/CSurface::scaleY)/TILE_SIZE;
	int j=(mX/CSurface::scaleX)/TILE_SIZE;
	if(mX >= (SCREEN_WIDTH*CSurface::scaleX - TILE_SIZE)/2  && mX <= (SCREEN_WIDTH*CSurface::scaleX + TILE_SIZE)/2 && i==0)
		Running=2;
}
//==============================================================================
