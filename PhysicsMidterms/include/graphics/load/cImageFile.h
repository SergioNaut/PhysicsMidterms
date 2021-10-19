#pragma once
#include <extern_includes.h>

#include <string>

namespace nGraphics
{
	class cImageFile
	{
	public:
		cImageFile();
		~cImageFile();

		cImageFile(const cImageFile& other) = delete;
		cImageFile& operator=(const cImageFile& other) = delete;

		bool IsLoaded();
		void Unload();
		bool LoadFromFile(const std::string& fileName);
		GLubyte* GetTextureData();
		int GetImageHeight();
		int GetImageWidth();
		int GetBitsPerPixel();
	private:
		
		
		FIBITMAP* mBitmap;
		FIBITMAP* mBitmap32;
		int mBitsPerPixel;
		int mImageWidth;
		int mImageHeight;
	};
}