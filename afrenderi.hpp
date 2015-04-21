
#pragma once
#include "include/SDL_video.h"
#include "afiqe.hpp"
#include "af3dmath.hpp"

typedef struct afRenderer {
	SDL_Window * window;
	SDL_GLContext context;
	void (*init)(afRenderer * self, SDL_Window * window);
	void (*beginFrame)(afRenderer * self);
	void (*endFrame)(afRenderer * self);
	void (*begin3D)(afRenderer * self);
	void (*drawModel)(afRenderer * self, afModel*);
	void (*begin2D)(afRenderer * self);
	afMat4 projection, modelview;
} afRenderer;

extern afRenderer afR;

inline
void afRInit(SDL_Window * window){
	afR.init(&afR, window);
}

inline
void afRBeginFrame(){
	afR.beginFrame(&afR);
}

inline
void afREndFrame(){
	afR.endFrame(&afR);
}

inline
void afRBegin3D(){
	afR.begin3D(&afR);
}

inline
void afRDrawModel(afModel* model){
	afR.drawModel(&afR, model);
}

inline
void afRBegin2D(){
	afR.begin2D(&afR);
}


