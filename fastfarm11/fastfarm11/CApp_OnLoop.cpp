//==============================================================================
#include "CApp.h"

//==============================================================================
void CApp::OnLoop() {
	
	CSeedlingW::no_seedling+= CPlant::OnLoopStatic();

	for(int i = 0;i < CEntity::EntityList.size();i++) {
    if(!CEntity::EntityList[i]) continue;
	
    CEntity::EntityList[i]->OnLoop();

	if(CEntity::EntityList[i]->dead) {
		CEntity::EntityList[i]->OnCleanup();
		delete CEntity::EntityList[i];
		CEntity::EntityList[i] = NULL;
		CEntity::EntityList.erase(CEntity::EntityList.begin()+i);
		continue;
	}
	}

	try {
		SDL_DestroyTexture(textTex);
		textTex = NULL;
		char str[200];
		sprintf(str,"Moves = %d \nPurchases = %d \nStock = %d",CSeedlingW::moves,CSeedlingW::seedling_purch,CSeedlingW::no_seedling);
		textTex = CSurface::OnLoadText(str,"../CooperBlackStd.otf", 110,236,4, 22,ren,600);
	}
	catch (const std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
}

//==============================================================================
