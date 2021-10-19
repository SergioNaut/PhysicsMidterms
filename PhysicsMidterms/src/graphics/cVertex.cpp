#include <graphics/cVertex.h>

namespace nGraphics
{
	cVertex::cVertex()
		: Position(0.f, 0.f, 0.f)
		, PositionW(1.f)
		, Normal(1.f, 0.f, 0.f)
		, NormalW(1.f)
		, TexCoords(0.f, 0.f, 0.f, 0.f) {}
}