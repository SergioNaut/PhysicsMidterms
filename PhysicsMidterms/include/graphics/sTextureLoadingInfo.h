#pragma once

// std
#include <string>

namespace nGraphics
{
	struct sTextureLoadingInfo
	{
		sTextureLoadingInfo() : IsSkybox(false) {}
		std::string File;
		std::string Name;
		bool IsSkybox;
	};
}