#include <extern_includes.h>
#include <graphics/gl_errors.h>

// std
#include <string>

namespace nGraphics
{
	void CheckErrors()
	{
		GLenum glError = glGetError();
		if (glError)
		{
			std::string errorString("There was an error loading the texture: ");

			switch (glError)
			{
			case GL_INVALID_ENUM:
			{
				errorString += "Invalid enum.";
			}
			break;
			case GL_INVALID_VALUE:
			{
				errorString += "Invalid value.";
			}
			break;
			case GL_INVALID_OPERATION:
			{
				errorString += "Invalid operation.";
			}
			break;
			default:
			{
				errorString += "Unrecognised GLenum.";
			}
			break;
			}

			// See https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml for further details.;
		}
	}
}