#pragma once
#include <extern_includes.h>

// std
#include <string>

namespace nGraphics
{
	class cTextureManager;
	class cTexture
	{
	public:
		inline bool IsCubeMap() const { return mIsCubeMap; }
		inline GLuint GetId() const { return mId; }
		inline const std::string& GetFile() { return mFile; }
		inline const std::string& GetName() { return mName; }
		inline void Bind() { glBindTexture(GL_TEXTURE_2D, mId); }
	private:
		friend class cTextureManager;
		// cTextureManager is entirely responsible for the life-cycle
		cTexture(const std::string& file, const std::string& name, GLuint id, bool isCubeMap);
		~cTexture();
		GLuint mId;
		std::string mFile;
		std::string mName;
		bool mIsCubeMap;
		cTexture(const cTexture& other) {}
		cTexture& operator=(const cTexture& other) { return *this; }
	};
}