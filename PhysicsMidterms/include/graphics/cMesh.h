#pragma once

#include <graphics/shader/shader_vars.h>

// std
#include <string>

namespace nGraphics
{
	class cMeshManager;
	class cMesh
	{
	public:
		//sPerEntityVars Vars;
		// getters
		// the values are meant to stay constant
		// so no touchy-touchy after the initial setup is done
		inline int GetNumIndices()	const { return mNumIndices; }
		inline int GetBaseIndex()	const { return mBaseIndex; }
		inline int GetBaseVertex()	const { return mBaseVertex; }
		inline const std::string& GetName() const { return mName; }
		bool Bind();
		bool Render();
	private:
		friend class cMeshManager;
		// cMeshManager is entirely responsible for the life-cycle
		cMesh(cMeshManager* manager, const std::string& name, int numIndices, int baseIndex, int baseVertex);
		~cMesh();

		std::string mName;

		int mNumIndices;
		int mBaseIndex;
		int mBaseVertex;

		cMeshManager* mManager;

		cMesh(cMesh& other) {}
		cMesh& operator=(cMesh& other) { return *this; }
	};
}