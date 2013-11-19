//==============================================================================
#include "CApp.h"

//==============================================================================
CApp::CApp(SDL_Renderer* ren1, SDL_Window* win1) {
	window = win1;
	ren = ren1;
    Running = 1;
	textTex = nullptr;
	level =1;
	crop = nullptr;
}

//------------------------------------------------------------------------------
int CApp::OnExecute() {
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
		OnGameOver();
    }

    OnCleanup();

	return Running;
}

//==============================================================================
/*int main(int argc, char* argv[]) {
    CApp theApp;

    return theApp.OnExecute();
}
*/
//==============================================================================
