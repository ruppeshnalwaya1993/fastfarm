#include "CAnimation.h"
 
CAnimation::CAnimation() {
    CurrentFrame    = 0;
    MaxFrames       = 0;
    FrameInc        = 1;
	wait_time		= 0;  //ms
    FrameRate       = 100; //Milliseconds
    OldTime         = 0;
 
    Oscillate       = false;
}
 
void CAnimation::OnAnimate() {

	if(MaxFrames==0) {//CurrentFrame = 0;
	return;}

    if(OldTime + FrameRate > SDL_GetTicks()) {
        return;
    }
	if(CurrentFrame == 0 && OldTime + wait_time>SDL_GetTicks())
		return;
    OldTime = SDL_GetTicks();
 
    CurrentFrame += FrameInc;
 
    if(Oscillate) {
        if(FrameInc > 0) {
            if(CurrentFrame >= MaxFrames-1) {
                FrameInc = -FrameInc;
				CurrentFrame = MaxFrames-1;
            }
        }else{
            if(CurrentFrame <= 0) {
                FrameInc = -FrameInc;
				CurrentFrame = 0;
            }
        }
    }else{
		if(CurrentFrame > MaxFrames-1 || CurrentFrame < 0) {
            CurrentFrame = 0;
        }
    }
}
 
void CAnimation::SetFrameRate(int Rate) {
    FrameRate = Rate;
}
 
void CAnimation::SetCurrentFrame(int Frame) {
    if(Frame < 0 || Frame > MaxFrames) return;
 
    CurrentFrame = Frame;
}
 
int CAnimation::GetCurrentFrame() {
    return CurrentFrame;
}