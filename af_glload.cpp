
#include "af_glload.hpp"
#include <signal.h>
static bool afGLIsInit = false;

//vbo
glGenBuffersFunc		glGenBuffers=0;
glBindBufferFunc		glBindBuffer=0;
glBufferDataFunc		glBufferData=0;
glDeleteBuffersFunc		glDeleteBuffers=0;
//shaders
glCreateShaderFunc		glCreateShader=0;
glShaderSourceFunc		glShaderSource=0;
glCompileShaderFunc		glCompileShader=0;
glGetShaderivFunc		glGetShaderiv=0;
glIsShaderFunc			glIsShader=0;
glGetShaderInfoLogFunc		glGetShaderInfoLog=0;
glDeleteShaderFunc		glDeleteShader=0;
glGetAttribLocationFunc 	glGetAttribLocation=0;
glEnableVertexAttribArrayFunc 	glEnableVertexAttribArray=0;
glVertexAttribPointerFunc 	glVertexAttribPointer=0;
glDisableVertexAttribArrayFunc 	glDisableVertexAttribArray=0;
//programs
glCreateProgramFunc		glCreateProgram=0;
glAttachShaderFunc		glAttachShader=0;
glLinkProgramFunc		glLinkProgram=0;
glGetProgramivFunc		glGetProgramiv=0;
glUseProgramFunc		glUseProgram=0;
glIsProgramFunc			glIsProgram=0;
glGetProgramInfoLogFunc		glGetProgramInfoLog=0;
glDeleteProgramFunc		glDeleteProgram=0;

void afLoadGL()
{
	if (afGLIsInit) return;
	afGLIsInit = true;
	//glggg = (glgggFunc)SDL_GL_GetProcAddress("glggg");
	
	//	VBO
	glGenBuffers = (glGenBuffersFunc)SDL_GL_GetProcAddress("glGenBuffers");
	if (!glGenBuffers ) printf("glGenBuffers  not supported\n");
	glBufferData = (glBufferDataFunc)SDL_GL_GetProcAddress("glBufferData");
	if (!glBufferData ) printf("glBufferData  not supported\n");
	glBindBuffer = (glBindBufferFunc)SDL_GL_GetProcAddress("glBindBuffer");
	if (!glBindBuffer ) printf("glBindBuffer  not supported\n");
	glDeleteBuffers = (glDeleteBuffersFunc)SDL_GL_GetProcAddress("glDeleteBuffers");
	if (!glDeleteBuffers ) printf("glDeleteBuffers  not supported\n");

	//	Shader
	glCreateShader = (glCreateShaderFunc)SDL_GL_GetProcAddress("glCreateShader");
	if (!glCreateShader ) printf("glCreateShader  not supported\n");
	glShaderSource = (glShaderSourceFunc)SDL_GL_GetProcAddress("glShaderSource");
	if (!glShaderSource ) printf("glShaderSource  not supported\n");
	glCompileShader = (glCompileShaderFunc)SDL_GL_GetProcAddress("glCompileShader");
	if (!glCompileShader ) printf("glCompileShader  not supported\n");
	glGetShaderiv = (glGetShaderivFunc)SDL_GL_GetProcAddress("glGetShaderiv");
	if (!glGetShaderiv ) printf("glGetShaderiv  not supported\n");
	glIsShader = (glIsShaderFunc)SDL_GL_GetProcAddress("glIsShader");
	if (!glIsShader ) printf("glIsShader  not supported\n");
	glGetShaderInfoLog = (glGetShaderInfoLogFunc)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	if (!glGetShaderInfoLog ) printf("glGetShaderInfoLog  not supported\n");
	glDeleteShader = (glDeleteShaderFunc)SDL_GL_GetProcAddress("glDeleteShader");
	if (!glDeleteShader ) printf("glDeleteShader  not supported\n");
	glGetAttribLocation = (glGetAttribLocationFunc)SDL_GL_GetProcAddress("glGetAttribLocation");
	if (!glGetAttribLocation ) printf("glGetAttribLocation  not supported\n");
	glEnableVertexAttribArray = (glEnableVertexAttribArrayFunc)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	if (!glEnableVertexAttribArray ) printf("glEnableVertexAttribArray  not supported\n");
	glVertexAttribPointer = (glVertexAttribPointerFunc)SDL_GL_GetProcAddress("glVertexAttribPointer");
	if (!glVertexAttribPointer ) printf("glVertexAttribPointer  not supported\n");
	glDisableVertexAttribArray = (glDisableVertexAttribArrayFunc)SDL_GL_GetProcAddress("glDisableVertexAttribArray");
	if (!glDisableVertexAttribArray ) printf("glDisableVertexAttribArray  not supported\n");

	//	Program
	glCreateProgram = (glCreateProgramFunc)SDL_GL_GetProcAddress("glCreateProgram");
	if (!glCreateProgram ) printf("glCreateProgram  not supported\n");
	glAttachShader = (glAttachShaderFunc)SDL_GL_GetProcAddress("glAttachShader");
	if (!glAttachShader ) printf("glAttachShader  not supported\n");
	glLinkProgram = (glLinkProgramFunc)SDL_GL_GetProcAddress("glLinkProgram");
	if (!glLinkProgram ) printf("glLinkProgram  not supported\n");
	glGetProgramiv = (glGetProgramivFunc)SDL_GL_GetProcAddress("glGetProgramiv");
	if (!glGetProgramiv ) printf("glGetProgramiv  not supported\n");
	glUseProgram = (glUseProgramFunc)SDL_GL_GetProcAddress("glUseProgram");
	if (!glUseProgram ) printf("glUseProgram  not supported\n");
	glIsProgram = (glIsProgramFunc)SDL_GL_GetProcAddress("glIsProgram");
	if (!glIsProgram ) printf("glIsProgram  not supported\n");
	glGetProgramInfoLog = (glGetProgramInfoLogFunc)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	if (!glGetProgramInfoLog ) printf("glGetProgramInfoLog  not supported\n");
	glDeleteProgram = (glDeleteProgramFunc)SDL_GL_GetProcAddress("glDeleteProgram");
	if (!glDeleteProgram ) printf("glDeleteProgram  not supported\n");
}
