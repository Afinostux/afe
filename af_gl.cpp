
#include "af_gl.hpp"
#include "include/SDL_video.h"
#include "include/SDL_rwops.h"


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


GLuint afGetShaderTypeByExtension(const char *fname)
{
	uint result, fnameidx=0;
	while (fname[fnameidx] != '\0') fnameidx++;
	while (fname[fnameidx] != '.') fnameidx--;

	//fprintf(stderr,"ext %s\n", &fname[fnameidx]);
	if (strcmp(".frag",&fname[fnameidx]) == 0){
		result = FRAGMENT_SHADER;
	} else if (strcmp(".vert",&fname[fnameidx]) == 0) {
		result = VERTEX_SHADER;
	} else {
		result = 0;
	}

	return result;
}

GLuint afLoadShader(const char *fname)
{
	uint type = afGetShaderTypeByExtension(fname);
	if (!type) return 0;

	GLuint result = glCreateShader(type);
	if (!result) return 0;

	SDL_RWops * shaderFile = SDL_RWFromFile(fname,"r");
	if (!shaderFile) return 0;

	size_t size = SDL_RWsize(shaderFile);

	char *fcontents = (char*)malloc(size);
	const GLchar * source[1] = {fcontents};
	
	SDL_RWread(shaderFile, fcontents, size, 1);

	glShaderSource(result, 1, source, 0);
	//free((void*)source);
	free((void*)fcontents);

	glCompileShader(result);

	GLint compile_ok = GL_FALSE;
	glGetShaderiv(result, GL_COMPILE_STATUS, &compile_ok);

	if (!compile_ok) {
		fprintf(stderr,"failed to load %s\n", fname);
		afDebugShaderObject(result);
		glDeleteShader(result);
		result = 0;
	}
	
	SDL_RWclose(shaderFile);
	return result;
}

GLuint afMakeGLProgram(GLuint vs, GLuint ps){
	if (vs == 0 || ps == 0) return 0;
	GLuint result = glCreateProgram();
	glAttachShader(result, vs);
	glAttachShader(result, ps);
	glLinkProgram(result);
	
	GLint compile_ok = GL_FALSE;
	glGetProgramiv(result, GL_LINK_STATUS, &compile_ok);

	if (!compile_ok)
	{
		fprintf(stderr, "problem in program %u:\n", result);
		afDebugShaderObject(result);
		glDeleteProgram(result);
	}

	return result;
}

void afDebugShaderObject(GLuint oid){
	GLint logLength;
	char * logGL;
	if (glIsShader(oid)){
		glGetShaderiv(oid, GL_INFO_LOG_LENGTH, &logLength);
		logGL = (char*)malloc(logLength);
		glGetShaderInfoLog(oid, logLength, NULL, logGL);
	} else if (glIsProgram(oid)) {
		glGetProgramiv(oid, GL_INFO_LOG_LENGTH, &logLength);
		logGL = (char*)malloc(logLength);
		glGetProgramInfoLog(oid, logLength, NULL, logGL);
	} else {
		fprintf(stderr,"shameful!\n");
		return;
	}
	fprintf(stderr, "%s\n", logGL);
	free(logGL);
}
