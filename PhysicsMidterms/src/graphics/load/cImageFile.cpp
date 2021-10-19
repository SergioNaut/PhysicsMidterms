#pragma once
#include <extern_includes.h>
#include <graphics/load/cImageFile.h>

// std
#include <iostream>

namespace nGraphics
{

	cImageFile::cImageFile()
		: mBitmap(0)
		, mBitmap32(0)
		, mImageWidth(0)
		, mImageHeight(0)
		, mBitsPerPixel(0)
	{

	}
	cImageFile::~cImageFile()
	{
		Unload();
	}
	bool cImageFile::IsLoaded()
	{
		return mBitmap != 0;
	}
	void cImageFile::Unload()
	{
		if (!IsLoaded())
		{
			return;
		}
		// Unload the 32-bit colour bitmap
		FreeImage_Unload(mBitmap32);

		// If we had to do a conversion to 32-bit colour, then unload the original
		// non-32-bit-colour version of the image data too. Otherwise, bitmap32 and
		// bitmap point at the same data, and that data's already been free'd, so
		// don't attempt to free it again! (or we'll crash).
		if (mBitsPerPixel != 32)
		{
			FreeImage_Unload(mBitmap);
		}
		mBitmap = 0;
		mBitmap32 = 0;
		mImageHeight = 0;
		mImageWidth = 0;
		mBitsPerPixel = 0;
	}
	bool cImageFile::LoadFromFile(const std::string& fileName)
	{
		Unload(); // safety first
		// Determine the format of the image.
		// Note: The second paramter ('size') is currently unused, and we should use 0 for it.
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName.c_str(), 0);

		// Image not found? Abort! Without this section we get a 0 by 0 image with 0 bits-per-pixel but we don't abort, which
		// you might find preferable to dumping the user back to the desktop.
		if (format == -1)
		{
			std::cout << "Could not find image: " << fileName << " - Aborting." << std::endl;;
			return false;
		}

		// Found image, but couldn't determine the file format? Try again...
		if (format == FIF_UNKNOWN)
		{
			std::cout << "Couldn't determine file format - attempting to get from file extension..." << std::endl;

			// ...by getting the filetype from the filename extension (i.e. .PNG, .GIF etc.)
			// Note: This is slower and more error-prone that getting it from the file itself,
			// also, we can't use the 'U' (unicode) variant of this method as that's Windows only.
			format = FreeImage_GetFIFFromFilename(fileName.c_str());

			// Check that the plugin has reading capabilities for this format (if it's FIF_UNKNOWN,
			// for example, then it won't have) - if we can't read the file, then we bail out =(
			if (!FreeImage_FIFSupportsReading(format))
			{
				std::cout << "Detected image format cannot be read!" << std::endl;
				return false;
			}
		}

		// If we're here we have a known image format, so load the image into a bitap
		mBitmap = FreeImage_Load(format, fileName.c_str());

		// How many bits-per-pixel is the source image?
		mBitsPerPixel = FreeImage_GetBPP(mBitmap);

		// Convert our image up to 32 bits (8 bits per channel, Red/Green/Blue/Alpha) -
		// but only if the image is not already 32 bits (i.e. 8 bits per channel).
		// Note: ConvertTo32Bits returns a CLONE of the image data - so if we
		// allocate this back to itself without using our bitmap32 intermediate
		// we will LEAK the original bitmap data, and valgrind will show things like this:
		//
		// LEAK SUMMARY:
		//  definitely lost: 24 bytes in 2 blocks
		//  indirectly lost: 1,024,874 bytes in 14 blocks    <--- Ouch.
		//
		// Using our intermediate and cleaning up the initial bitmap data we get:
		//
		// LEAK SUMMARY:
		//  definitely lost: 16 bytes in 1 blocks
		//  indirectly lost: 176 bytes in 4 blocks
		//
		// All above leaks (192 bytes) are caused by XGetDefault (in /usr/lib/libX11.so.6.3.0) - we have no control over this.
		//
		
		if (mBitsPerPixel == 32)
		{
			std::cout << "Source image has " << std::to_string(mBitsPerPixel) << " bits per pixel. Skipping conversion." << std::endl;;
			mBitmap32 = mBitmap;
		}
		else
		{
			std::cout << "Source image has " << std::to_string(mBitsPerPixel) << " bits per pixel. Converting to 32-bit colour." << std::endl;
			mBitmap32 = FreeImage_ConvertTo32Bits(mBitmap);
		}

		// Some basic image info - strip it out if you don't care
		mImageWidth = FreeImage_GetWidth(mBitmap32);
		mImageHeight = FreeImage_GetHeight(mBitmap32);
		std::cout << "Image: " << fileName << " is size: " << std::to_string(mImageWidth) << "x" << std::to_string(mImageHeight) << "." << std::endl;;

		return true;
	}
	GLubyte* cImageFile::GetTextureData()
	{
		// Get a pointer to the texture data as an array of unsigned bytes.
		// Note: At this point bitmap32 ALWAYS holds a 32-bit colour version of our image - so we get our data from that.
		// Also, we don't need to delete or delete[] this textureData because it's not on the heap (so attempting to do
		// so will cause a crash) - just let it go out of scope and the memory will be returned to the stack.

		if (mBitmap32)
		{
			return FreeImage_GetBits(mBitmap32);;
		}
		else
		{
			return 0;
		}
	}
	int cImageFile::GetImageHeight()
	{
		return mImageHeight;
	}
	int cImageFile::GetImageWidth()
	{
		return mImageWidth;
	}
	int cImageFile::GetBitsPerPixel()
	{
		return mBitsPerPixel;
	}
}