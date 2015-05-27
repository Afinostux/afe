
#pragma once
#include "SDL/SDL_video.h"
#include "afiqe.hpp"
#include "af3dmath.hpp"

typedef struct afRenderer {
	SDL_Window * window;
	SDL_GLContext context;
	virtual
	void init(SDL_Window * window)=0;

	virtual
	void beginFrame()=0;

	virtual
	void endFrame()=0;

	virtual
	void begin3D()=0;

	virtual
	void drawModel(afModel*)=0;

	virtual
	void drawModelSkin(afModel*, afPoseBuffer*)=0;

	virtual
	void begin2D()=0;

	afMat4 projection, modelview;
} afRenderer;

extern afRenderer* afR;

#define afRInit(window) afR->init(window);

#define afRBeginFrame() afR->beginFrame();

#define afREndFrame() afR->endFrame();

#define afRBegin3D() afR->begin3D();

#define afRDrawModel(model) afR->drawModel(model);

#define afRDrawModelSkin(model, pose_buffer) afR->drawModelSkin(model, pose_buffer);

#define afRBegin2D() afR->begin2D();


