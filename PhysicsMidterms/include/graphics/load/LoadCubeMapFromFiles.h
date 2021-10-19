#pragma once
#include <extern_includes.h>

namespace nGraphics
{
	GLuint LoadCubeMapFromFiles(
		const char* posX_fileName, const char* negX_fileName,
		const char* posY_fileName, const char* negY_fileName,
		const char* posZ_fileName, const char* negZ_fileName,
		bool isSeamless);
}