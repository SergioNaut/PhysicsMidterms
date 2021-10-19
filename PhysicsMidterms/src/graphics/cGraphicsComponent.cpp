#include <graphics/cGraphicsComponent.h>
#include <graphics/graphics.h>

namespace nGraphics
{
	cGraphicsComponent::cGraphicsComponent(const sGraphicsComponentDef& def)
	{
		if (def.Mesh != "")
		{
			SetMesh(def.Mesh);
		}
		if (def.TexCubeMap != "")
		{
			mVars.CubeMap = gTextureManager->GetTextureByName(def.TexCubeMap);
			mVars.IsCubeMap = true;
		}
		if (def.TexDiffuse != "")
		{
			SetDiffuseTexture(def.TexDiffuse);
		}
		mVars.ModelMatrix = glm::eulerAngleXYZ(def.Rotation.x, def.Rotation.y, def.Rotation.z);
		mVars.ModelMatrix = glm::translate(mVars.ModelMatrix, def.Position);
		mVars.ModelMatrix = glm::scale(mVars.ModelMatrix, def.Scale);
	}
	cGraphicsComponent::cGraphicsComponent()
		: mMesh(0)
	{

	}

	cGraphicsComponent::~cGraphicsComponent()
	{
		mMesh = 0;
	}

	bool cGraphicsComponent::SetMesh(const std::string& meshName)
	{
		mMesh = nGraphics::gMeshManager->GetMeshByName(meshName);
		return mMesh != 0;
	}

	bool cGraphicsComponent::SetDiffuseTexture(const std::string& texName, float factor)
	{
		mVars.TexDiffuse = nGraphics::gTextureManager->GetTextureByName(texName);
		if (mVars.TexDiffuse)
		{
			mVars.TexFactors.x = factor;
			return true;
		}
		else
		{
			mVars.TexFactors.x = 0.f;
			return false;
		}

	}

	void cGraphicsComponent::Render()
	{
		if (!mMesh) return;
		nGraphics::Render(mMesh, mVars);
	}
}