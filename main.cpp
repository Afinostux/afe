#include <stdio.h>
#include <stdlib.h>
#include "include/SDL.h"
#include "include/SDL_audio.h"
#include "af_gl.hpp"
#include "af_keys.hpp"
#include "af_audio.hpp"
#include "af_3dmath.hpp"

#define TAU 6.283185307
typedef unsigned int uint;

const uint afFrameTargetMS = 30;
bool running = true;

int main (const int argc, const char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER);

	SDL_AudioSpec want, have;
	SDL_zero(want);
	want.freq = 48000;
	want.format = AUDIO_F32;
	want.channels = 1;
	want.samples = 800;

	afAudioInit(&want, &have);

#if 0
// vim macro play area
//	1--------------0
// w = f1jlr1k0f0jhr00
// a = yyp:s/[10]/-/gk@w
// s = :s/1/2:s/0/1:s/2/0
// d = 15@a@s
#endif
	afMat4 junk;
	junk.print();
	junk.rotate(TAU/4, 0.f, 0.f);
	junk.print();
	junk.translate(1.f, 2.f, 3.f);
	junk.print();
	junk.rotate(TAU/4, 0.f, 0.f);
	junk.print();
	printf("%f\n",junk.determinant());
//
//
//

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


	afInitGL(window);

	GLuint
		vertShader = afLoadShader("basic.vert"),
		fragShader = afLoadShader("basic.frag");

	GLuint
		shaderProgram = afMakeGLProgram(vertShader, fragShader);

	SDL_Event event;
	
	glClearColor(0,0,0,1);

	GLuint vbuffer=0, cbuffer;
	GLint vcolorattrib;
	glGenBuffers(1,&vbuffer);
	glGenBuffers(1,&cbuffer);

	GLfloat triangle[9] = {
		0,	1.f,	0,
		0.4f,	-1.0f,	0,
		-1.f,	-1.0f,	0,
	};

	GLfloat colors[9] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
	};

	if (!vbuffer || !cbuffer || !shaderProgram) {running = false;}

	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*9, triangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*9, colors, GL_STATIC_DRAW);

	vcolorattrib = glGetAttribLocation(shaderProgram, "v_color");
	if (vcolorattrib == -1) {
		fprintf(stderr, "v_color failed");
		running = false;
	}

	afeKey *gkey = afGetKey(SDLK_g);
	afeKey *nkey = afGetKey(SDLK_n);

	uint last_tick, current_tick = SDL_GetTicks();
	unsigned long frame_start_perf, frame_end_perf,
		      perf_frequency = SDL_GetPerformanceFrequency();

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
		if (nkey->pressed && nkey->pressedms == 0) afNewSynthSquare(0.f, 0.1f, 261.6, 0.5);
		if (gkey->pressedms > 300) running = false;

		afGLStartFrame();
		afGLSet3D();
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -10.f);

		glUseProgram(shaderProgram);

		glEnableVertexAttribArray(vcolorattrib);
		glBindBuffer(GL_ARRAY_BUFFER, cbuffer);
		glVertexAttribPointer(vcolorattrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableClientState(GL_VERTEX_ARRAY);

		glDisableVertexAttribArray(vcolorattrib);


		afGLEndFrame();


		frame_end_perf = SDL_GetPerformanceCounter();
		uint frametime = (frame_end_perf - frame_start_perf) / (perf_frequency / 1000);

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

	if (vbuffer)
		glDeleteBuffers(1,&vbuffer);

	SDL_GL_DeleteContext(afPrimaryGlContext);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
