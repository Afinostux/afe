
#pragma once

#include "afglload.hpp"

enum afShaderTypes{
	FAIL_SHADER=0,
	VERTEX_SHADER=GL_VERTEX_SHADER, FRAGMENT_SHADER=GL_FRAGMENT_SHADER
};

extern 
SDL_GLContext afPrimaryGlContext;


bool afInitGL(SDL_Window * window);
GLuint afGetShaderTypeByExtension(const char *fname);
GLuint afLoadShader(const char *fname);
GLuint afMakeGLProgram(GLuint vs, GLuint ps, GLuint attribute_count, const GLchar ** attribute_list, GLint * attribute_out);
void afDebugShaderObject(GLuint oid);
