#include <stdio.h>
#include <stdlib.h>
#include "include/SDL.h"
#include "include/SDL_audio.h"
#include "afmath.hpp"
#include "afgl.hpp"
#include "afkeys.hpp"
#include "afaudio.hpp"
#include "af3dmath.hpp"
#include "afiqe.hpp"
#include "afrendergl2.hpp"

//I was always told not to do this,
//but goddamn if it doesn't make the compile time own bwns
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

	afModel * cuber = afLoadIQE("cube.iqe");

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
		if (nkey->pressed && nkey->pressedms == 0) afNewSynthSquare(0, 0.1, 261.6, 0.01);

		junk = afMat4::identity;
		junk.rotate(rotationphase/100, rotationphase/10, 0);
		junk.translate( sin(rotationphase/10),cos(rotationphase/10), -10);
		//junk.translate(sin(rotationphase/1000) * 10, sin(rotationphase/100) * 10, sin(rotationphase/10) * 10);
		rotationphase += 1;


		afRBeginFrame();
		afRBegin3D();
		glLoadMatrixf(junk.m);
		afRDrawModel(cuber);
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
	glGetFloatv(GL_PROJECTION_MATRIX, junk.m);
	junk.print();

	SDL_GL_DeleteContext(afPrimaryGlContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
