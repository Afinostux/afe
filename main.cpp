#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_audio.h>
#include "af_gl.hpp"
#include "af_keys.h"

typedef unsigned int uint;

bool running = true;
bool donoise = false;
SDL_AudioDeviceID adevice = 0;
float volume = 0.1;

int piphase = 0;

const uint audio_hz = 48000;


void afAudioCallback(void *udata, Uint8 *stream, int len)
{
	static uint
		currentAudio = SDL_GetTicks(),
		prevAudio = currentAudio;

	SDL_memset(stream, 0, len);
	float *FAB = (float*)stream;
	const int FABsize = len/sizeof(float);

	if (donoise) {
		for (int phase = 0; phase < FABsize; phase++)
		{
			FAB[phase] = (((piphase/100)%2 == 0)?1:-1) * volume;
			piphase++;
		}
	}

	prevAudio = currentAudio;
	currentAudio = SDL_GetTicks();
}


int main (const int argc, const char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
//
//
//

	SDL_AudioSpec want, have;
	SDL_zero(want);
	want.freq = audio_hz;
	want.format = AUDIO_F32;
	want.channels = 1;
	want.samples = 1024;
	want.callback = afAudioCallback;

	adevice = SDL_OpenAudioDevice(
			NULL, 0,
			&want, &have,
			SDL_AUDIO_ALLOW_ANY_CHANGE);
	
	SDL_PauseAudioDevice(adevice, 0);
	
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


	SDL_GLContext con = SDL_GL_CreateContext(window);
	loadGL();

	GLuint
		vertShader = loadShader("basic.vert"),
		fragShader = loadShader("basic.frag");

	GLuint
		shaderProgram = makeGLProgram(vertShader, fragShader);

	SDL_Event event;
	
	glClearColor(0,0,0,1);

	GLuint vbuffer=0, cbuffer;
	GLint vcolorattrib;
	glGenBuffers(1,&vbuffer);
	glGenBuffers(1,&cbuffer);

	GLfloat triangle[9] = {
		0,	1.f,	0,
		0.4f,	-1.0f,	0,
		-0.4f,	-1.0f,	0,
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

	afeKey *gkey = getKey(SDLK_g);
	afeKey *noise = getKey(SDLK_n);

	uint last_tick, current_tick = SDL_GetTicks();

	while (running) {
		last_tick = current_tick;
		current_tick = SDL_GetTicks();
		tickKeys(current_tick - last_tick);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: {running = false;} break;
				case SDL_KEYDOWN:
				case SDL_KEYUP: {
							handleKeys(&event.key);
						}
					       

			}
		}
		if (gkey->pressedms > 1000) running = false;
		donoise = noise->pressed;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

		SDL_GL_SwapWindow(window);
		
		SDL_Delay(16);
	};

	if (vbuffer)
		glDeleteBuffers(1,&vbuffer);

	SDL_GL_DeleteContext(con);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
