#pragma once
#include <graphics/cVertex.h>
#include <graphics/load/sMeshLoadingInfo.h>
#include <vector>

namespace nGraphics
{
	
	struct sMeshEntry
	{
		sMeshEntry();
		void Clear();
		
		std::string Name;

		int NumIndices;
		int BaseIndex;
		int BaseVertex;
	};
	bool LoadMultipleMeshesFromFile(
		const char * path,
		::std::vector<unsigned int> & indices,
		::std::vector<cVertex>& vertices,
		::std::vector<sSubMeshLoadingInfo>& meshInfos,
		std::vector<sMeshEntry>& meshesOut,
		sMeshEntry& fullInfoOut
	);
	bool LoadSingleMeshFromFile(
		const char * path,
		::std::vector<unsigned int> & indices,
		::std::vector<cVertex>& vertices,
		sMeshEntry& meshOut,
		unsigned int meshIndex = 0
	);
	bool ResizeAndCenter(
		::std::vector<cVertex>& vertices,
		sMeshEntry& mesh,
		const ::glm::vec3& resizeDimensions
	);
	bool ResizeAndCenter(
		::std::vector<cVertex>& vertices,
		sMeshEntry& mesh,
		float maxRadialSize
	);
}