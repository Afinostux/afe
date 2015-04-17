
#pragma once

#include "af_glload.hpp"

enum afShaderTypes{
	FAIL_SHADER=0,
	VERTEX_SHADER=GL_VERTEX_SHADER, FRAGMENT_SHADER=GL_FRAGMENT_SHADER
};

extern 
SDL_GLContext afPrimaryGlContext;


bool afInitGL(SDL_Window * window);
void afGLStartFrame();
void afGLSet2D();
void afGLSet3D();
void afGLEndFrame();
GLuint afGetShaderTypeByExtension(const char *fname);
GLuint afLoadShader(const char *fname);
GLuint afMakeGLProgram(GLuint vs, GLuint ps);
void afDebugShaderObject(GLuint oid);
