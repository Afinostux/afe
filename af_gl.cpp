
#include "af_gl.hpp"
#include "include/SDL_video.h"
#include "include/SDL_rwops.h"

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

void afGLFrustum(float *matrix,
		float left, float right,
		float bottom, float top,
		float near, float far){
	float 
	twicenear = 2 * near,
	width = right - left,
	height = top - bottom,
	depth = far - near;
	matrix[0] = twicenear / width;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;
	matrix[4] = 0.0f;
	matrix[5] = twicenear / height;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
	matrix[8] = (right + left) / width;
	matrix[9] = (top + bottom) / height;
	matrix[10] = (-far - near) / depth;
	matrix[11] = -1.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (-twicenear * far) / depth;
	matrix[15] = 0.0f;
}

void afGLPerspective(float *matrix,
		float fovy, float aspect,
		float znear, float zfar) {
	float
	ymax = znear * tanf(fovy * M_PI / 360.0),
	xmax = ymax * aspect;
	afGLFrustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

SDL_Window * afGLWindow;

bool afInitGL(SDL_Window * window)
{
	afPrimaryGlContext = SDL_GL_CreateContext(window);
	afGLWindow = window;
	afLoadGL();
	glClearColor(0,0,0,1);

	return 0;
}

void afGLStartFrame(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void afGLSet2D(){
	glDisable(GL_DEPTH_TEST);
}


void afGLSet3D(){
	int w, h;
	glEnable(GL_DEPTH_TEST);

	SDL_GetWindowSize(afGLWindow, &w, &h);
	
	float perspective[16];
	afGLPerspective(perspective, 90.f, (float)w / (float)h, 1.f, 100.f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(perspective);
	glMatrixMode(GL_MODELVIEW);
}

void afGLEndFrame(){
	SDL_GL_SwapWindow(afGLWindow);
}

