
#include "afgl.hpp"
#include "SDL/SDL_video.h"
#include "SDL/SDL_rwops.h"

SDL_GLContext afPrimaryGlContext = 0;



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

	int size = SDL_RWsize(shaderFile);

	unsigned char *fcontents = (unsigned char*)malloc(size+1);
	SDL_RWread(shaderFile, fcontents, size+1, 1);
	fcontents[size] = 0;

	bool ispound = false;

#if 0
	for (int i = 0; i < size; i++){
		if (ispound){
			if (fcontents[i] == '\n') ispound = false;
		} else {
			if (fcontents[i] == '#') ispound = false;
			if (fcontents[i] == '\n') fcontents[i] = ' ';
		}
	}
#endif

	glShaderSource(result, 1, (const GLchar**)&fcontents, 0);
	//free((void*)source);

	glCompileShader(result);

	GLint compile_ok = GL_FALSE;
	glGetShaderiv(result, GL_COMPILE_STATUS, &compile_ok);


	if (!compile_ok) {
		fprintf(stderr,"%s\n", fcontents);
		fprintf(stderr,"failed to load %s\n", fname);
		SDL_RWops * debugout = SDL_RWFromFile("ERROR.txt","w");
		SDL_RWwrite(debugout, fcontents, 1, size);
		SDL_RWclose(debugout);
		afDebugShaderObject(result);
		glDeleteShader(result);
		result = 0;
	}
	
	free((void*)fcontents);
	SDL_RWclose(shaderFile);
	return result;
}

GLuint afMakeGLProgram(GLuint vs, GLuint ps, GLuint attribute_count, const GLchar ** attribute_list, GLint * attribute_out)
{
	if (vs == 0 || ps == 0) return 0;
	GLuint result = glCreateProgram();
	glAttachShader(result, vs);
	glAttachShader(result, ps);

	while (attribute_count--){
		attribute_out[attribute_count] = glGetAttribLocation(result, attribute_list[attribute_count]);
		if (attribute_out[attribute_count] == -1) {
			fprintf(stderr, "%s failed\n", attribute_list[attribute_count]);
		}
	}

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

SDL_Window * afGLWindow;

