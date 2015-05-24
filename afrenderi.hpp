
#pragma once
#include "SDL/SDL_video.h"
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
	void (*drawModelSkin)(afRenderer * self, afModel*, afPoseBuffer*);
	void (*begin2D)(afRenderer * self);
	afMat4 projection, modelview;
} afRenderer;

extern afRenderer afR;

#define afRInit(window) afR.init(&afR, window);

#define afRBeginFrame() afR.beginFrame(&afR);

#define afREndFrame() afR.endFrame(&afR);

#define afRBegin3D() afR.begin3D(&afR);

#define afRDrawModel(model) afR.drawModel(&afR, model);

#define afRDrawModelSkin(model, pose_buffer) afR.drawModelSkin(&afR, model, pose_buffer);

#define afRBegin2D() afR.begin2D(&afR);


