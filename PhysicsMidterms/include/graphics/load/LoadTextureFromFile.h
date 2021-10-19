#pragma once
#include <extern_includes.h>
//#ifndef FREEIMAGE_LIB
//#define FREEIMAGE_LIB
//#endif

namespace nGraphics
{
	GLuint LoadTextureFromFile(const char* fileName, GLenum minificationFilter = GL_LINEAR, GLenum magnificationFilter = GL_LINEAR);
}