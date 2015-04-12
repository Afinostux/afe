#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "af_gl.hpp"

typedef unsigned int uint;

bool running = true;

int main (const int argc, const char ** argv)
{
	SDL_Init(SDL_INIT_VIDEO);

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

	while (running) {
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: {running = false;}
			}
		}
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
		
		SDL_Delay(10);
	};

	if (vbuffer)
		glDeleteBuffers(1,&vbuffer);

	SDL_GL_DeleteContext(con);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
