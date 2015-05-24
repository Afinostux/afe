#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
#include "afmath.hpp"
#include "afgl.hpp"
#include "afkeys.hpp"
#include "afaudio.hpp"
#include "af3dmath.hpp"
#include "afiqe.hpp"
#include "afrendergl2.hpp"

//NOTE(afox): source files go here
#include "afglload.cpp"
#include "afgl.cpp"
#include "af3dmath.cpp"
#include "afaudio.cpp"
#include "afcstrings.cpp"
#include "afiqe.cpp"
#include "afkeys.cpp"
#include "afmath.cpp"
#include "afrenderi.cpp"
#include "afrendergl2.cpp"


const uint afFrameTargetMS = 5;
bool running = true;

int main (const int argc, const char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER);

	SDL_AudioSpec want, have;
	SDL_zero(want);
	want.freq = 48000;
	want.format = AUDIO_F32;
	want.channels = 1;
	want.samples = 400;

	afAudioInit(&want, &have);

	SDL_Window *window;

	window = SDL_CreateWindow(
			"hello world",
		       	SDL_WINDOWPOS_UNDEFINED,
		       	SDL_WINDOWPOS_UNDEFINED,
		       	640, 480,
		       	SDL_WINDOW_OPENGL
		       	);

	if (!window) {
		fprintf(stderr,"window fucked up: %s\n", SDL_GetError());
		return 1;
	}

	afModel * cuber = afLoadIQE("wiggler.iqe");
	afPoseBuffer pose = cuber->getPoseBuffer();

	afR = createRendererG2(afPrimaryGlContext);
	afRInit(window);

	SDL_Event event;
	
	afeKey *gkey = afGetKey(SDLK_g);
	afeKey *nkey = afGetKey(SDLK_n);

	uint last_tick, current_tick = SDL_GetTicks();
	unsigned long frame_start_perf, frame_end_perf,
		      perf_frequency = SDL_GetPerformanceFrequency();

	afMat4 junk;

	float rotationphase = 0.f;

	afQuat junkQuat;

	while (running) {
		frame_start_perf = SDL_GetPerformanceCounter();
		last_tick = current_tick;
		current_tick = SDL_GetTicks();
		afPushAudio(0.001*(current_tick - last_tick));

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: {running = false;} break;
				case SDL_KEYDOWN:
				case SDL_KEYUP: {
							afHandleKeys(&event.key);
						}
			}
		}

		if (gkey->pressed) running = false;
		if (nkey->pressed && nkey->pressedms == 0) afNewSynthSquare(0, 1, 261.6, 0.01);

#if 0
		junk = afMat4::identity;
		junk.rotate(rotationphase/100, rotationphase/10, 0);
		junk.translate( sin(rotationphase/10),cos(rotationphase/10), -10);
		//junk.translate(sin(rotationphase/1000) * 10, sin(rotationphase/100) * 10, sin(rotationphase/10) * 10);
#else
		junkQuat = {0, 0, 0, 1};
		//junkQuat = junkQuat.rotateByEuler( rotationphase/1000, rotationphase/100, 0);
		junkQuat = junkQuat.rotateByEuler( 0, 90, 0);
		junk = junkQuat.toMatrix();
		//junk.translate( sin(rotationphase/100),cos(rotationphase/100), -10);
		junk.translate( 0, 0, -10);
		if (pose.posecount) {
			pose.poses[1].rotation = pose.poses[1].rotation.rotateByEuler(0.1, 0, 0);
			printf("%f %f %f %f\n", 
					pose.poses[1].rotation.w,
					pose.poses[1].rotation.x,
					pose.poses[1].rotation.y,
					pose.poses[1].rotation.z
					);
		}
#endif
		rotationphase += 1;


		afRBeginFrame();
		afRBegin3D();
		glLoadMatrixf(junk.m);
		afRDrawModelSkin(cuber, &pose);
		//afRDrawModel(cuber);
		afREndFrame();

		frame_end_perf = SDL_GetPerformanceCounter();
		uint frametime = (frame_end_perf - frame_start_perf) / (perf_frequency / 1000.f);

		if (frametime < afFrameTargetMS) {
#if 0
			fprintf(stderr,
				"(%u)\n",
				frametime);
#endif
			while (frametime < afFrameTargetMS) {
				frame_end_perf = SDL_GetPerformanceCounter();
				frametime = (frame_end_perf - frame_start_perf) / (perf_frequency / 1000);
			}
		}
		else
		{//missed frame target
#if 0
			fprintf(stderr,
				"missed frame target (%u, %1.1f)\n",
				frametime, (float)frametime / afFrameTargetMS);
#endif
		}
		afTickKeys(frametime);
	};

	pose.clear();
	glGetFloatv(GL_PROJECTION_MATRIX, junk.m);
	junk.print();

	SDL_GL_DeleteContext(afPrimaryGlContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
