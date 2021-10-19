#include <extern_includes.h>
#include <graphics/gl_errors.h>
#include <graphics/load/LoadCubeMapFromFiles.h>
#include <graphics/load/cImageFile.h>

namespace nGraphics
{
	GLuint LoadCubeMapFromFiles(
		const char* posX_fileName, const char* negX_fileName,
		const char* posY_fileName, const char* negY_fileName,
		const char* posZ_fileName, const char* negZ_fileName,
		bool isSeamless)
	{

		// Generate a texture ID and bind to it
		GLuint tempTextureID;
		glGenTextures(1, &tempTextureID);
		glActiveTexture(GL_TEXTURE2);
		CheckErrors();
		glBindTexture(GL_TEXTURE_CUBE_MAP, tempTextureID);
		CheckErrors();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
		//glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		CheckErrors();
		if (isSeamless)
		{
			//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);	// OpenGL 4.1, maybe
		}

		CheckErrors();

		cImageFile* image = new cImageFile();
		// Positive X image...
		// Assume all the images are the same size. If not, then it will screw up
		if (image->LoadFromFile(posX_fileName))
		{
			glTexStorage2D(GL_TEXTURE_CUBE_MAP,
				10, // Mipmap levels
				GL_RGBA8,	// Internal format
				image->GetImageWidth(),	// width
				image->GetImageHeight()); // height (pixels)
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
			0,   // Level
			0, 0, // Offset
			image->GetImageWidth(),	// width
			image->GetImageHeight(), // height
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image->GetTextureData());
		CheckErrors();

		// Negative X image...
		if (image->LoadFromFile(negX_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
				0, 0, 0,
				image->GetImageWidth(),	// width
				image->GetImageHeight(), // height
				GL_RGBA, // shoudl this be GL_RGBA??
				GL_UNSIGNED_BYTE, 
				image->GetTextureData());
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		// Positive Y image...
		if (image->LoadFromFile(posY_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				0, 0, 0,
				image->GetImageWidth(),	// width
				image->GetImageHeight(), // height
				GL_RGBA, // shoudl this be GL_RGBA??
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		// Negative Y image...
		if (image->LoadFromFile(negY_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				0, 0, 0,
				image->GetImageWidth(),	// width
				image->GetImageHeight(), // height
				GL_RGBA, // shoudl this be GL_RGBA??
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		// Positive Z image...
		if (image->LoadFromFile(posZ_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				0, 0, 0,
				image->GetImageWidth(),	// width
				image->GetImageHeight(), // height
				GL_RGBA, // shoudl this be GL_RGBA??
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		// Negative Z image...
		if (image->LoadFromFile(negZ_fileName))
		{
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				0, 0, 0,
				image->GetImageWidth(),	// width
				image->GetImageHeight(), // height
				GL_RGBA, // shoudl this be GL_RGBA??
				GL_UNSIGNED_BYTE,
				image->GetTextureData());
			CheckErrors();
		}
		else
		{
			delete image;
			return 0;
		}

		delete image;
		// Finally, return the texture ID
		return tempTextureID;
	}
}