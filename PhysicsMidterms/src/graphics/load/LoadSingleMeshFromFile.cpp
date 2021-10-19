#include <graphics/load/LoadSingleMeshFromFile.h>
#include <graphics/load/Assimp.h>

// std
#include <iostream>

namespace nGraphics
{
	sMeshEntry::sMeshEntry() : NumIndices(0), BaseIndex(0), BaseVertex(0) {}
	void sMeshEntry::Clear()
	{
		NumIndices = 0;
		BaseIndex = 0;
		BaseVertex = 0;
	}
	const aiScene* InternalLoadScene(Assimp::Importer& importer, const char* path)
	{
		unsigned int flags = 0;
		//flags |= aiProcess_SortByPType;
		//flags |= aiProcess_LimitBoneWeights;
		//flags |= aiProcess_CalcTangentSpace;
		//flags |= aiProcess_JoinIdenticalVertices;
		//flags |= aiProcess_GenNormals;
		//flags |= aiProcess_GenUVCoords;
		//flags |= aiProcess_TransformUVCoords;
		flags |= aiProcess_GenSmoothNormals;
		flags |= aiProcess_Triangulate;
		//flags |= aiProcess_FindDegenerates;
		//flags |= aiProcess_FindInvalidData;
		const aiScene* scene = importer.ReadFile(path, flags);
		if (!scene)
		{
			std::cout << "Error loading scene from path '" << path << "'" << std::endl;
			std::cout << "Assimp Error '" << importer.GetErrorString() << "'" << std::endl;
		}
		return scene;
	}
	bool InternalLoadMesh(
		const aiMesh* mesh,
		::std::vector<unsigned int> & indices,
		::std::vector<cVertex>& vertices,
		sMeshEntry& meshOut
	)
	{
		if (!mesh) return false;
		meshOut.Clear();
		std::string meshName(mesh->mName.C_Str());
		unsigned int vertexOffset = vertices.size();
		unsigned int indexOffset = indices.size();
		unsigned int numVertices = mesh->mNumVertices;
		unsigned int numIndices = 3 * mesh->mNumFaces; // assume triangles

		meshOut.NumIndices = numIndices;
		meshOut.BaseIndex = indexOffset;
		meshOut.BaseVertex = vertexOffset;

		// Fill vertices positions
		vertices.resize(vertexOffset + numVertices);
		for (unsigned int i = 0; i<mesh->mNumVertices; i++)
		{
			aiVector3D pos = mesh->mVertices[i];
			aiVector3D n = mesh->mNormals[i];

			cVertex& vert = vertices[vertexOffset + i];
			glm::set(vert.Position, pos.x, pos.y, pos.z);
			glm::set(vert.TexCoords, 0.f, 0.f, 0.f);
			glm::set(vert.Normal, n.x, n.y, n.z);
		}
		if (mesh->HasTextureCoords(0))
		{
			for (unsigned int i = 0; i<mesh->mNumVertices; i++)
			{
				aiVector3D uvw = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.

				cVertex& vert = vertices[vertexOffset + i];
				glm::set(vert.TexCoords, uvw.x, uvw.y, uvw.z);
			}
		}

		// Fill face indices
		indices.resize(indexOffset + numIndices);
		for (unsigned int i = 0; i<mesh->mNumFaces; i++)
		{
			unsigned int baseInd = 3 * i;
			// Assume the model has only triangles.
			indices[indexOffset + baseInd] = mesh->mFaces[i].mIndices[0];
			indices[indexOffset + baseInd + 1] = mesh->mFaces[i].mIndices[1];
			indices[indexOffset + baseInd + 2] = mesh->mFaces[i].mIndices[2];
		}

		// The "scene" pointer will be deleted automatically by "importer"
		return true;
	}
	bool LoadMultipleMeshesFromFile(
		const char * path,
		::std::vector<unsigned int> & indices,
		::std::vector<cVertex>& vertices,
		::std::vector<sSubMeshLoadingInfo>& meshInfos,
		std::vector<sMeshEntry>& meshesOut,
		sMeshEntry& fullInfoOut
	)
	{
		meshesOut.clear();
		fullInfoOut.Clear();

		Assimp::Importer importer;
		const aiScene* scene = InternalLoadScene(importer, path);
		if (!scene)
		{
			return false;
		}
		if (!scene->HasMeshes() || scene->mNumMeshes == 0)
		{
			return false;
		}
		bool success = true;
		bool firstMesh = true;
		if (path == "assets/L200/L200-OBJ.obj")
		{
			bool sexybreakpoint = true;
		}
		::std::vector<sSubMeshLoadingInfo>::iterator itInfo = meshInfos.begin();
		while (itInfo != meshInfos.end())
		{
			sMeshEntry meshEntry;
			
			if (scene->mNumMeshes > itInfo->MeshIndex && InternalLoadMesh(scene->mMeshes[itInfo->MeshIndex], indices, vertices, meshEntry))
			{
				meshEntry.Name = itInfo->Name;
				if (firstMesh)
				{
					firstMesh = false;
					fullInfoOut.BaseIndex = meshEntry.BaseIndex;
					fullInfoOut.BaseVertex = meshEntry.BaseVertex;
				}
				fullInfoOut.NumIndices += meshEntry.NumIndices;
				meshesOut.push_back(meshEntry);
			}
			else
			{
				success = false;
			}
			++itInfo;
		}
		return success;
	}

	bool LoadSingleMeshFromFile(
		const char * path,
		::std::vector<unsigned int> & indices,
		::std::vector<cVertex>& vertices,
		sMeshEntry& meshOut,
		unsigned int meshIndex
	)
	{
		Assimp::Importer importer;
		const aiScene* scene = InternalLoadScene(importer, path);
		if (!scene)
		{
			return false;
		}
		
		if (scene->HasMeshes() && scene->mNumMeshes > meshIndex)
		{
			return InternalLoadMesh(scene->mMeshes[meshIndex], indices, vertices, meshOut);
		}
		else
		{
			return false;
		}
	}

	bool ResizeAndCenter(
		::std::vector<cVertex>& vertices,
		sMeshEntry& mesh,
		const glm::vec3& resizeDimensions
	)
	{
		// build up an axis aligned bounding box
		glm::aabb box;
		unsigned int totalVerts = vertices.size();
		for (unsigned int idx = (unsigned int)mesh.BaseVertex; idx < totalVerts; ++idx)
		{
			box.inflate(vertices[idx].Position);
		}
		// get the center
		glm::vec3 center;
		box.getCenter(center);
		
		glm::vec3 size;
		box.getSize(size);

		glm::vec3 rescale = resizeDimensions / size;

		// move it over, and rescale
		for (unsigned int idx = (unsigned int)mesh.BaseVertex; idx < totalVerts; ++idx)
		{
			vertices[idx].Position -= center;
			vertices[idx].Position *= rescale;
		}
		return true;
	}
	bool ResizeAndCenter(
		::std::vector<cVertex>& vertices,
		sMeshEntry& mesh,
		float maxRadialSize
	)
	{
		// build up an axis aligned bounding box
		glm::aabb box;
		unsigned int totalVerts = vertices.size();
		for (unsigned int idx = (unsigned int)mesh.BaseVertex; idx < totalVerts; ++idx)
		{
			box.inflate(vertices[idx].Position);
		}
		// get the center
		glm::vec3 center;
		box.getCenter(center);

		glm::vec3 size;
		box.getSize(size);

		float maxAxisSize = glm::max(glm::max(size.x, size.y), size.z);
		float rescale = maxRadialSize / maxAxisSize;

		// move it over, and rescale
		for (unsigned int idx = (unsigned int)mesh.BaseVertex; idx < totalVerts; ++idx)
		{
			vertices[idx].Position -= center;
			vertices[idx].Position *= rescale;
		}
		return true;
	}
}