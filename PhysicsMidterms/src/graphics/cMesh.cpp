#include <graphics/cMesh.h>
#include <graphics/cMeshManager.h>

namespace nGraphics
{
	cMesh::cMesh(cMeshManager* manager, const std::string& name, int numIndices, int baseIndex, int baseVertex)
		: mManager(manager), mName(name)
		, mNumIndices(numIndices), mBaseIndex(baseIndex), mBaseVertex(baseVertex)
	{

	}

	cMesh::~cMesh()
	{
		mManager = 0;
	}

	bool cMesh::Bind()
	{
		return mManager->Bind(this);
	}

	bool cMesh::Render()
	{
		return mManager->Render(this);
	}
}