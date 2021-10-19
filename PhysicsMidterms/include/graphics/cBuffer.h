#pragma once
#include <graphics/cMesh.h>
#include <graphics/cVertex.h>
#include <vector>

namespace nGraphics
{
	/**
		describes in individual attribute
	*/
	class cVertexAttributeDesc
	{
	public:
		// default setup is for a vec4 (float, float, float, float)
		cVertexAttributeDesc() : ByteSize(4 * sizeof(float)), NumVariables(4), Type(0x1406), Normalize(false) {}
		cVertexAttributeDesc(int byteSize, int numVariables, unsigned int type, bool normalize)
			: ByteSize(byteSize), NumVariables(numVariables), Type(type), Normalize(normalize) {}
		~cVertexAttributeDesc() {}

		int ByteSize;		// default 4*sizeof(float) for vec4
		int NumVariables;	// default 4 for a vec4
		unsigned int Type;  // default 0x1406 (GL_FLOAT) for vec4
		bool Normalize;		// normalize the values
	};
	/**
		helper struct to contain buffer ids
	*/
	struct sBufferIds
	{
		sBufferIds() : VertexId(0), IndexId(0), BufferIds(0), NumBufferIds(0) {}

		void Clear()
		{
			VertexId = 0;
			IndexId = 0;
			if (BufferIds) delete BufferIds;
			BufferIds = 0;
			NumBufferIds = 0;
		}

		void Destroy()
		{
			glDeleteBuffers(1, &VertexId);
			glDeleteBuffers(1, &IndexId);
			if (NumBufferIds > 0)
			{
				glDeleteBuffers(NumBufferIds, BufferIds);
				delete BufferIds;
			}
			VertexId = 0;
			IndexId = 0;
			BufferIds = 0;
		}

		GLuint VertexId;
		GLuint IndexId;
		GLuint* BufferIds;
		int NumBufferIds;
	};

	template<class VerType>
	class cVertexDescT : public std::vector<cVertexAttributeDesc>
	{
	public:
		cVertexAttributeDesc Vertex;

		cVertexDescT() : mVertexByteSize(sizeof(VerType)), Vertex() {};

		inline int GetVertexByteSize() const { return mVertexByteSize; }

		bool GenerateBuffers(sBufferIds& ids, std::vector<VerType>& vertices, std::vector<unsigned int>& indices, GLenum usage)
		{
			glGenVertexArrays(1, &ids.VertexId);
			glBindVertexArray(ids.VertexId);

			ids.NumBufferIds = size();
			if (ids.NumBufferIds > 0)
			{
				ids.BufferIds = new GLuint[ids.NumBufferIds];

				glGenBuffers(ids.NumBufferIds, ids.BufferIds);
				for (int idxId = 0; idxId < ids.NumBufferIds; idxId++)
				{
					glBindBuffer(GL_ARRAY_BUFFER, ids.BufferIds[idxId]);
				}
			}

			int bytesInVertexArray = vertices.size() * mVertexByteSize;
			glBufferData(GL_ARRAY_BUFFER, bytesInVertexArray, &vertices[0], usage);

			glGenBuffers(1, &ids.IndexId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids.IndexId);
			//if (nGL::IsThereAnError("cVertexAttributeDesc::GenerateBuffers"))
				//return false;

			int bytesInIndexArray = indices.size() * sizeof(GLuint);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, bytesInIndexArray, &indices[0], GL_STATIC_DRAW);

			return true;
		}

		bool BindBuffers(sBufferIds& ids)
		{
			glBindVertexArray(ids.VertexId);
			for (int idx = 0; idx < ids.NumBufferIds; idx++)
				glBindBuffer(GL_ARRAY_BUFFER, ids.BufferIds[idx]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids.IndexId);
			// ids.NumBufferIds doesn't include the vertex, so we start at 0 and go to <= ids.NumBufferIds
			for (int idx = 0; idx <= ids.NumBufferIds; idx++)
				glEnableVertexAttribArray(idx);

			glVertexAttribPointer(0,		// index or "slot" in the shader
				Vertex.NumVariables,		// Number of variables: vec4 would be 4 32-bit variables
				Vertex.Type,				// Type: vec4 is float
				Vertex.Normalize,			// "normalize" the values (or not)
				mVertexByteSize,			// Number of bytes per vertex (the "stride")
				(GLvoid*)0);				// Offset from vertex (position is first, so offset = 0)

			int offset = Vertex.ByteSize;

			for (int idx = 0; idx < ids.NumBufferIds; idx++)
			{
				cVertexAttributeDesc& attrib = at(idx);
				glVertexAttribPointer(idx + 1,
					attrib.NumVariables,
					attrib.Type,
					attrib.Normalize,
					mVertexByteSize,
					(GLvoid*)offset);
				offset += attrib.ByteSize;
			}
			return true;
		}

		bool DisableBuffers(sBufferIds& ids)
		{
			glDisableVertexAttribArray(0);
			for (int idx = 0; idx < ids.NumBufferIds; idx++)
			{
				glDisableVertexAttribArray(idx + 1);
			}
			glBindVertexArray(0);
			return true;
		}
	private:
		int mVertexByteSize;
	};
	/*
		cBuffer
		really just a container for a few different buffer-related helper classes
		and a container for mesh entries for reference
		load, bind, disable, render functionality
	*/
	class cBuffer
	{
	public:
		~cBuffer();

		static cBuffer* CreateBuffer(std::vector<unsigned int> & indices,
			std::vector<cVertex>& vertices,
			std::vector<cMesh*>& meshes);

		bool Bind();
		bool Disable();
		// render the whole buffer
		bool Render();
		// render mesh id
		bool Render(int meshIndex);
		bool Render(int numIndices, int baseIndex, int baseVertex);
		bool Render(cMesh* mesh);
		cMesh* GetMesh(unsigned int index);
		cMesh* GetMeshByName(const std::string& name);
		inline bool IsLoaded() const { return mIds != 0; }
		inline unsigned int NumMeshes() const { return mMeshes.size(); }
		inline unsigned int NumVertices() const { return mNumVertices; }
		inline unsigned int NumIndices() const { return mNumIndices; }
	private:

		cBuffer();
		std::vector<cMesh*> mMeshes;
		cVertexDescT<cVertex> mDesc;
		sBufferIds* mIds;
		unsigned int mNumVertices;
		unsigned int mNumIndices;

		cBuffer(cBuffer& other) {}
		cBuffer& operator=(cBuffer& other) { return *this; }
	};
}