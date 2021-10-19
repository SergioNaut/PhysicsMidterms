#pragma once
#include <graphics/cMesh.h>
#include <graphics/shader/cSimpleShader.h>
#include <graphics/shader/shader_vars.h>

namespace nGraphics
{
	struct sGraphicsComponentDef
	{
		sGraphicsComponentDef()
			: ModelColor(1.f, 1.f, 1.f, 1.f)
			, TexFactors(0.f, 0.f, 0.f, 0.f)
			, Position(0.f, 0.f, 0.f)
			, Rotation(0.f, 0.f, 0.f)
			, Scale(1.f, 1.f, 1.f)
		{

		}
		std::string Mesh;
		std::string TexDiffuse;
		std::string TexCubeMap;
		glm::vec4 ModelColor;
		glm::vec4 TexFactors;
		glm::vec3 Position;
		glm::vec3 Rotation;
		glm::vec3 Scale;
	};
	class cGraphicsComponent
	{
	public:
		cGraphicsComponent(const sGraphicsComponentDef& def);
		cGraphicsComponent();
		~cGraphicsComponent();

		inline nGraphics::cMesh* GetMesh() { return mMesh; }
		inline nGraphics::sPerEntityVars* GetVars() { return &mVars; }

		// helpers - retrieves from global graphics managers
		bool SetMesh(const std::string& meshName);
		bool SetDiffuseTexture(const std::string& texName, float factor = 1.f);

		void Render();
	private:
		cGraphicsComponent(const cGraphicsComponent& other) {}
		cGraphicsComponent& operator=(const cGraphicsComponent& other) { return *this; }

		nGraphics::cMesh*			mMesh;
		nGraphics::sPerEntityVars   mVars;
	};
}