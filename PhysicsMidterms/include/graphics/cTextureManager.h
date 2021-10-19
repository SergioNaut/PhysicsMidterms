#pragma once
#include <extern_includes.h>
#include <graphics/sTextureLoadingInfo.h>
#include <graphics/cTexture.h>

// std
#include <vector>

namespace nGraphics
{
	// singleton
	class cTextureManager
	{
	public:
		
		static cTextureManager* GetInstance();
		~cTextureManager();

		// load an individual texture
		cTexture* Load(const std::string& file, const std::string& name, bool isSkybox = false);
		inline cTexture* Load(const sTextureLoadingInfo& loadingInfo)
		{
			return Load(loadingInfo.File, loadingInfo.Name, loadingInfo.IsSkybox);
		}
		// load everything
		bool Load(std::vector<sTextureLoadingInfo>& loadingInfos);
		// unload everything
		void Unload();
		// if it hasn't been loaded, 0 will be returned
		cTexture* GetTextureByName(const std::string& name);
		cTexture* GetTextureByFile(const std::string& file);
		cTexture* GetTextureById(GLuint id);

	private:
		typedef std::vector<cTexture*>::iterator texerator;
		cTextureManager();
		cTextureManager(const cTextureManager& tm) {}
		cTextureManager& operator=(const cTextureManager& tm) { return *this; }

		static cTextureManager* mInstance;
		std::vector<cTexture*> mTextures;
	};
}