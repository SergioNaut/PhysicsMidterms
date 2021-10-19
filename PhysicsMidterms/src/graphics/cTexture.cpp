#include <graphics/cTexture.h>

namespace nGraphics
{
	cTexture::cTexture(const std::string& file, const std::string& name, GLuint id, bool isCubeMap)
		: mFile(file), mName(name), mId(id), mIsCubeMap(isCubeMap) {}

	cTexture::~cTexture()
	{
		glDeleteTextures(1, &mId);
	}
}