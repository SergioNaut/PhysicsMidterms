#include <graphics/cMeshManager.h>
#include <graphics/load/LoadSingleMeshFromFile.h>

// std
#include <iostream>


namespace nGraphics
{
	cMeshManager* cMeshManager::mInstance = 0;

	cMeshManager* cMeshManager::GetInstance()
	{
		if (mInstance == 0)
		{
			mInstance = new cMeshManager();
		}
		return mInstance;
	}

	cMeshManager::cMeshManager() 
	: mBuffer(0)
	{

	}

	cMeshManager::~cMeshManager()
	{
		Disable();
		Unload();
		mInstance = 0;
	}

	bool cMeshManager::Load(std::vector<sMeshLoadingInfo>& loadingInfos)
	{
		bool result(true);
		Unload();
		
		std::vector<cVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<sMeshLoadingInfo>::iterator it = loadingInfos.begin();
		while (it != loadingInfos.end())
		{
			std::vector<sMeshEntry> entries;
			sMeshEntry fullEntry;
			if (!LoadMultipleMeshesFromFile(it->File.c_str(), indices, vertices, it->SubMeshes, entries, fullEntry))
			{
				result = false;
			}
			else
			{
				std::cout << it->File << " loaded successfully" << std::endl;
				if (it->DoResize)
				{
					if (it->MaintainDimensions)
					{
						float maxExtent = glm::min(glm::min(it->Extents.x, it->Extents.y), it->Extents.z);
						ResizeAndCenter(vertices, fullEntry, maxExtent);
					}
					else
					{
						ResizeAndCenter(vertices, fullEntry, it->Extents);
					}
				}
				std::vector<sMeshEntry>::iterator itEntry = entries.begin();
				while (itEntry != entries.end())
				{
					std::cout << itEntry->Name << " is a mesh!" << std::endl;
					mMeshes.push_back(new cMesh(this, itEntry->Name, itEntry->NumIndices, itEntry->BaseIndex, itEntry->BaseVertex));
					++itEntry;
				}
			}
			it++;
		}
		mBuffer = cBuffer::CreateBuffer(indices, vertices, mMeshes);
		return result;
	}

	void cMeshManager::Unload()
	{
		std::vector<cMesh*>::iterator it = mMeshes.begin();
		while (it != mMeshes.end())
		{
			delete (*it);
			it++;
		}
		mMeshes.clear();
		if (mBuffer)
		{
			mBuffer->Disable();
			delete mBuffer;
			mBuffer = 0;
		}
	}

	cMesh* cMeshManager::GetMeshByName(const std::string& name)
	{
		std::vector<cMesh*>::iterator it = mMeshes.begin();
		while (it != mMeshes.end())
		{
			if ((*it)->mName == name)
			{
				return *it;
			}
			it++;
		}
		return 0;
	}

	bool cMeshManager::Bind(cMesh* mesh)
	{
		if (mBuffer)
		{
			return mBuffer->Bind();
		}
		return false;
	}

	bool cMeshManager::Disable()
	{
		if (mBuffer)
		{
			return mBuffer->Disable();
		}
		return true;  // nothing do disable, always disabled!
	}

	bool cMeshManager::Render(cMesh* mesh)
	{
		if (mBuffer)
		{
			return mBuffer->Render(mesh->GetNumIndices(), mesh->GetBaseIndex(), mesh->GetBaseVertex());
		}
		return false;
	}
}