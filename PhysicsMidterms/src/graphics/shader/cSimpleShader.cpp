#include <graphics/shader/cSimpleShader.h>
#include <graphics/shader/LoadShader.h>

// std
#include <iostream>

namespace nGraphics
{
	cSimpleShader::cSimpleShader()
		: mProgramId(0)
		, mUniformId_ModelMatrix(0), mUniformId_ViewMatrix(0), mUniformId_ProjectionMatrix(0)
		, mUniformId_ModelColor(0), mUniformId_EyePosition(0)
		, mUniformId_TexFactors(0), mUniformId_TexDiffuse(0)
	{

	}

	cSimpleShader::~cSimpleShader()
	{
		Close();
	}

	bool cSimpleShader::Load()
	{
		if (IsLoaded()) return true; // all good
		mProgramId = LoadShader("../Assets/shaders/SimpleShader.vert", "../Assets/shaders/SimpleShader.frag");
		if (mProgramId == 0)
		{
			std::cout << "shader couldn't load" << std::endl;
			return false;
		}
		// per frame
		mUniformId_ViewMatrix = glGetUniformLocation(mProgramId, "ViewMatrix");
		mUniformId_ProjectionMatrix = glGetUniformLocation(mProgramId, "ProjectionMatrix");
		mUniformId_EyePosition = glGetUniformLocation(mProgramId, "EyePosition");
		// per item
		mUniformId_ModelMatrix = glGetUniformLocation(mProgramId, "ModelMatrix");
		mUniformId_ModelColor = glGetUniformLocation(mProgramId, "ModelColor");
		mUniformId_TexFactors = glGetUniformLocation(mProgramId, "TexFactors");
		mUniformId_TexDiffuse = glGetUniformLocation(mProgramId, "TexDiffuse");
		mUniformId_IsCubeMap = glGetUniformLocation(mProgramId, "IsCubeMap");
		mUniformId_CubeMap = glGetUniformLocation(mProgramId, "SamplerCube");
		return true;
	}

	void cSimpleShader::SetUniforms_PerFrame(sPerFrameVars& perFrameVars)
	{
		SetUniform_ProjectionMatrix(perFrameVars.ProjectionMatrix);
		SetUniform_ViewMatrix(perFrameVars.ViewMatrix);
		SetUniform_EyePosition(perFrameVars.EyePosition);
	}

	void cSimpleShader::SetUniforms_PerEntity(sPerEntityVars& perEntityVars)
	{
		SetUniforms_PerEntity(perEntityVars, perEntityVars.ModelMatrix);
	}

	void cSimpleShader::SetUniforms_PerEntity(sPerEntityVars& perEntityVars, const glm::mat4& modelMatrixOverride)
	{
		SetUniform_ModelMatrix(modelMatrixOverride);
		SetUniform_ModelColor(perEntityVars.ModelColor);
		SetUniform_TexFactors(perEntityVars.TexFactors);
		SetUniform_TexDiffuse(perEntityVars.TexDiffuse);
		//SetUniform_IsCubeMap(false);
		SetUniform_IsCubeMap(perEntityVars.IsCubeMap);
		SetUniform_CubeMap(perEntityVars.CubeMap);
		/*
		if (perEntityVars.IsCubeMap)
		{
			glDisable(GL_DEPTH);
			glDepthMask(GL_FALSE);
		}
		else
		{
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH);
		}
		*/
	}

	void cSimpleShader::Bind()
	{
		glUseProgram(mProgramId);
	}

	void cSimpleShader::Close()
	{
		if (mProgramId == 0) return;
		glDeleteShader(mProgramId);
		mProgramId = 0;
		mUniformId_ModelMatrix = 0;
		mUniformId_ModelColor = 0;
		mUniformId_ViewMatrix = 0;
		mUniformId_ProjectionMatrix = 0;
		mUniformId_EyePosition = 0;
		mUniformId_TexFactors = 0;
		mUniformId_TexDiffuse = 0;
	}
}