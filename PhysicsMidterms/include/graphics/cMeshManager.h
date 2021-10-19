#pragma once
#include <extern_includes.h>

#include <graphics/load/sMeshLoadingInfo.h>

#include <graphics/cBuffer.h>

namespace nGraphics
{
	// singleton
	class cMeshManager
	{
	public:
		static cMeshManager* GetInstance();
		~cMeshManager();

		// load an individual mesh (no submeshes)
		//bool Load(const std::string& file);
		//bool Load(const std::string& file, const glm::vec3& extents, bool maintainDimensions);
		bool Load(const sMeshLoadingInfo& loadingInfo);
		// load everything
		bool Load(std::vector<sMeshLoadingInfo>& loadingInfos);
		// unload everything
		void Unload();
		// get a loaded mesh
		// if it hasn't been loaded, 0 will be returned
		cMesh* GetMeshByName(const std::string& name);
		bool Bind(cMesh* mesh);
		bool Disable();
		bool Render(cMesh* mesh);
	private:
		//typedef std::vector<cMesh*>::iterator texerator;
		cMeshManager();
		cMeshManager(const cMeshManager& tm) = delete;
		cMeshManager& operator=(const cMeshManager& tm) = delete;

		static cMeshManager* mInstance;
		cBuffer* mBuffer; // TODO: multiple buffers!
		std::vector<cMesh*> mMeshes;
	};
}