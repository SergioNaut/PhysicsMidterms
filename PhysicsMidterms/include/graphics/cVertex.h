#pragma once
#include <extern_includes.h>

namespace nGraphics
{
	/*
		a mesh vertex with a position, normal, texcoord0, texcoord1
		TODO: add more cool and useful stuff??
	*/
	class cVertex
	{
	public:
		cVertex();

		glm::vec3 Position;
		float PositionW;	// separated for convenience
		glm::vec3 Normal;
		float NormalW;		// separated for convenience
		glm::vec4 TexCoords;
	};
}