
#include "af_glload.hpp"
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
	glBufferData = (glBufferDataFunc)SDL_GL_GetProcAddress("glBufferData");
	glBindBuffer = (glBindBufferFunc)SDL_GL_GetProcAddress("glBindBuffer");
	glDeleteBuffers = (glDeleteBuffersFunc)SDL_GL_GetProcAddress("glDeleteBuffers");

	//	Shader
	glCreateShader = (glCreateShaderFunc)SDL_GL_GetProcAddress("glCreateShader");
	glShaderSource = (glShaderSourceFunc)SDL_GL_GetProcAddress("glShaderSource");
	glCompileShader = (glCompileShaderFunc)SDL_GL_GetProcAddress("glCompileShader");
	glGetShaderiv = (glGetShaderivFunc)SDL_GL_GetProcAddress("glGetShaderiv");
	glIsShader = (glIsShaderFunc)SDL_GL_GetProcAddress("glIsShader");
	glGetShaderInfoLog = (glGetShaderInfoLogFunc)SDL_GL_GetProcAddress("glGetShaderInfoLog");
	glDeleteShader = (glDeleteShaderFunc)SDL_GL_GetProcAddress("glDeleteShader");
	glGetAttribLocation = (glGetAttribLocationFunc)SDL_GL_GetProcAddress("glGetAttribLocation");
	glEnableVertexAttribArray = (glEnableVertexAttribArrayFunc)SDL_GL_GetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (glVertexAttribPointerFunc)SDL_GL_GetProcAddress("glVertexAttribPointer");
	glDisableVertexAttribArray = (glDisableVertexAttribArrayFunc)SDL_GL_GetProcAddress("glDisableVertexAttribArray");

	//	Program
	glCreateProgram = (glCreateProgramFunc)SDL_GL_GetProcAddress("glCreateProgram");
	glAttachShader = (glAttachShaderFunc)SDL_GL_GetProcAddress("glAttachShader");
	glLinkProgram = (glLinkProgramFunc)SDL_GL_GetProcAddress("glLinkProgram");
	glGetProgramiv = (glGetProgramivFunc)SDL_GL_GetProcAddress("glGetProgramiv");
	glUseProgram = (glUseProgramFunc)SDL_GL_GetProcAddress("glUseProgram");
	glIsProgram = (glIsProgramFunc)SDL_GL_GetProcAddress("glIsProgram");
	glGetProgramInfoLog = (glGetProgramInfoLogFunc)SDL_GL_GetProcAddress("glGetProgramInfoLog");
	glDeleteProgram = (glDeleteProgramFunc)SDL_GL_GetProcAddress("glDeleteProgram");
}
