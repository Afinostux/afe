#pragma once
#include <GL/gl.h>
#include <GL/glu.h>
#include "include/SDL_video.h"
//
//	VBO functions
//

//glGenBuffers
typedef void (APIENTRY * glGenBuffersFunc)(GLsizei n, GLuint *buffers);
extern glGenBuffersFunc glGenBuffers;

//glBindBuffer
typedef void (APIENTRY * glBindBufferFunc)(GLenum target, GLuint buffer);
extern glBindBufferFunc glBindBuffer;

//glBufferData
typedef void (APIENTRY * glBufferDataFunc)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
extern glBufferDataFunc glBufferData;

//glDeleteBuffers
typedef void (APIENTRY * glDeleteBuffersFunc)(GLsizei n, GLuint *buffers);
extern glDeleteBuffersFunc glDeleteBuffers;

//
//	Shader functions
//

//glCreateShader
typedef GLuint (APIENTRY * glCreateShaderFunc)(GLenum target);
extern glCreateShaderFunc glCreateShader;

//glShaderSource
typedef void (APIENTRY * glShaderSourceFunc)(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
extern glShaderSourceFunc glShaderSource;

//glCompileShader
typedef void (APIENTRY * glCompileShaderFunc)(GLuint shader);
extern glCompileShaderFunc glCompileShader;

//glGetShaderiv
typedef void (APIENTRY * glGetShaderivFunc)(GLuint shader, GLenum pname, GLint *params);
extern glGetShaderivFunc glGetShaderiv;

//glIsShader
typedef GLboolean (APIENTRY * glIsShaderFunc)(GLuint target);
extern glIsShaderFunc glIsShader;

//glGetShaderInfoLog
typedef void (APIENTRY * glGetShaderInfoLogFunc)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infolog);
extern glGetShaderInfoLogFunc glGetShaderInfoLog;

//glDeleteShader
typedef void (APIENTRY * glDeleteShaderFunc)(GLuint shader);
extern glDeleteShaderFunc glDeleteShader;

//glGetAttribLocation
typedef GLint (APIENTRY * glGetAttribLocationFunc)(GLuint program, const GLchar *name);
extern glGetAttribLocationFunc glGetAttribLocation;

//glEnableVertexAttribArray
typedef void (APIENTRY * glEnableVertexAttribArrayFunc)(GLuint index);
extern glEnableVertexAttribArrayFunc glEnableVertexAttribArray;

//glVertexAttribPointer
typedef void (APIENTRY * glVertexAttribPointerFunc)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
extern glVertexAttribPointerFunc glVertexAttribPointer;

//glDisableVertexAttribArray
typedef void (APIENTRY * glDisableVertexAttribArrayFunc)(GLuint index);
extern glDisableVertexAttribArrayFunc glDisableVertexAttribArray;

//
//	Shader program functions
//

//glCreateProgram
typedef GLuint (APIENTRY * glCreateProgramFunc)(void);
extern glCreateProgramFunc glCreateProgram;

//glAttachShader
typedef void (APIENTRY * glAttachShaderFunc)(GLuint program, GLuint shader);
extern glAttachShaderFunc glAttachShader;

//glLinkProgram
typedef void (APIENTRY * glLinkProgramFunc)(GLuint program);
extern glLinkProgramFunc glLinkProgram;

//glGetProgramiv
typedef void (APIENTRY * glGetProgramivFunc)(GLuint program, GLenum pname, GLint *params);
extern glGetProgramivFunc glGetProgramiv;

//glUseProgram
typedef void (APIENTRY * glUseProgramFunc)(GLuint program);
extern glUseProgramFunc glUseProgram;

//glIsProgram
typedef GLboolean (APIENTRY * glIsProgramFunc)(GLuint target);
extern glIsProgramFunc glIsProgram;

//glGetProgramInfoLog
typedef void (APIENTRY * glGetProgramInfoLogFunc)(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infolog);
extern glGetProgramInfoLogFunc glGetProgramInfoLog;

//glDeleteProgram
typedef void (APIENTRY * glDeleteProgramFunc)(GLuint program);
extern glDeleteProgramFunc glDeleteProgram;

//
//	General GL functions
//

#if 0
//glGetFloatV(GL_MODELVIEW_MATRIX, &junk.m);
typedef void (APIENTRY * glGetFloatvFunc)(GLenum pname, GLfloat * params);
extern glGetFloatvFunc glGetFloatv;

//glGetBooleanv
typedef void (APIENTRY * glGetBooleanvFunc)(GLenum pname, GLboolean * params);
extern glGetBooleanvFunc glGetBooleanv;

//glGetIntegerv
typedef void (APIENTRY * glGetIntegervFunc)(GLenum pname, GLint * params);
extern glGetIntegervFunc glGetIntegerv;

//glGetDoublev
typedef void (APIENTRY * glGetDoublevFunc)(GLenum pname, GLdouble * params);
extern glGetDoublevFunc glGetDoublev;
#endif

//
//	Function prototype
//

//glggg
//typedef void (APIENTRY * glgggFunc)(args);
//extern glgggFunc glggg;

void afLoadGL(void);
