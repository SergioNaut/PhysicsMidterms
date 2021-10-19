#pragma once
#include <extern_includes.h>

namespace nGraphics
{
	/**
		helper struct to contain buffer ids
		shouldn't be used outside of the graphics engine
	*/
	struct sBufferIds
	{
		sBufferIds() : VertexId(0), IndexId(0), BufferIds(0), NumBufferIds(0) {}

		void Clear()
		{
			VertexId = 0;
			IndexId = 0;
			if (BufferIds) delete BufferIds;
			BufferIds = 0;
			NumBufferIds = 0;
		}

		void Destroy()
		{
			glDeleteBuffers(1, &VertexId);
			glDeleteBuffers(1, &IndexId);
			if (NumBufferIds > 0)
			{
				glDeleteBuffers(NumBufferIds, BufferIds);
				delete BufferIds;
			}
			VertexId = 0;
			IndexId = 0;
			BufferIds = 0;
		}

		GLuint VertexId;
		GLuint IndexId;
		GLuint* BufferIds;
		int NumBufferIds;
	};
}