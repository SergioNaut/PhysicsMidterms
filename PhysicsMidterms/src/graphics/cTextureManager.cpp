#include <graphics/gl_errors.h>
#include <graphics/cTextureManager.h>
#include <graphics/load/LoadTextureFromFile.h>
#include <graphics/load/LoadCubeMapFromFiles.h>

namespace nGraphics
{
	cTextureManager* cTextureManager::mInstance = 0;

	cTextureManager* cTextureManager::GetInstance()
	{
		if (mInstance == 0)
		{
			mInstance = new cTextureManager();
		}
		return mInstance;
	}

	cTextureManager::cTextureManager()
	{
		// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_Initialise(true);
#endif
	}

	cTextureManager::~cTextureManager()
	{
		// call this ONLY when linking with FreeImage as a static library
#ifdef FREEIMAGE_LIB
		FreeImage_DeInitialise();
#endif

		Unload();
		mInstance = 0;
	}

	cTexture* cTextureManager::Load(const std::string& file, const std::string& name, bool isSkybox)
	{
		GLuint id(0);
		if (isSkybox)
		{
			std::string posY(file + "_dn.tga");
			std::string negY(file + "_up.tga");
			std::string posX(file + "_ft.tga");
			std::string negZ(file + "_lf.tga");
			std::string negX(file + "_bk.tga");
			std::string posZ(file + "_rt.tga");
			CheckErrors();
			id = LoadCubeMapFromFiles(posX.c_str(), negX.c_str(), posY.c_str(), negY.c_str(), posZ.c_str(), negZ.c_str(), true);
		}
		else
		{
			id = LoadTextureFromFile(file.c_str());
		}
		

		if (id == 0)
		{
			return nullptr;
		}

		cTexture* result(new cTexture(file, name, id, isSkybox));
		mTextures.push_back(result);
		return result;
	}

	bool cTextureManager::Load(std::vector<sTextureLoadingInfo>& loadingInfos)
	{
		CheckErrors();
		bool result(true);
		Unload();
		std::vector<sTextureLoadingInfo>::iterator it = loadingInfos.begin();
		while (it != loadingInfos.end())
		{
			cTexture* loadedTex(Load(*it));
			if (!loadedTex)
			{
				result = false;
			}
			it++;
		}
		return result;
	}

	void cTextureManager::Unload()
	{
		texerator it = mTextures.begin();
		while (it != mTextures.end())
		{
			delete (*it);
			it++;
		}
		mTextures.clear();
	}
	
	cTexture* cTextureManager::GetTextureByName(const std::string& name)
	{
		texerator it = mTextures.begin();
		while (it != mTextures.end())
		{
			if ((*it)->mName == name)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}

	cTexture* cTextureManager::GetTextureByFile(const std::string& file)
	{
		texerator it = mTextures.begin();
		while (it != mTextures.end())
		{
			if ((*it)->mFile == file)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}

	cTexture* cTextureManager::GetTextureById(GLuint id)
	{
		texerator it = mTextures.begin();
		while (it != mTextures.end())
		{
			if ((*it)->mId == id)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}
}