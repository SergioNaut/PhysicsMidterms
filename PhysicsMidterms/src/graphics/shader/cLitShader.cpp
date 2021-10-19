#include <graphics/shader/cLitShader.h>
#include <graphics/shader/LoadShader.h>

// std
#include <iostream>

namespace nGraphics
{
	cLitShader::sMaterialUniforms::sMaterialUniforms()
		: TexDiffuse(0)
		, TexSpecular(0)
		, Shininess(0) 
		, MeshColor(0)
	{}
	cLitShader::sDirLightUniforms::sDirLightUniforms()
		: Direction(0)
		, Ambient(0)
		, Diffuse(0)
		, Specular(0)
	{}
	cLitShader::sPointLightUniforms::sPointLightUniforms()
		: Position(0)
		, Constant(0)
		, Linear(0)
		, Quadratic(0)
		, Ambient(0)
		, Diffuse(0)
		, Specular(0) 
	{}
	cLitShader::sSpotLightUniforms::sSpotLightUniforms()
		: Position(0)
		, Direction(0)
		, CutOff(0)
		, OuterCutOff(0)
		, Constant(0)
		, Linear(0)
		, Quadratic(0)
		, Ambient(0)
		, Diffuse(0)
		, Specular(0) 
	{}

	cLitShader::cLitShader()
		: mProgramId(0)
		, mUniformId_ModelMatrix(0)
		, mUniformId_ViewMatrix(0)
		, mUniformId_ProjectionMatrix(0)
		, mUniformId_EyePosition(0)
		, mUniformId_IsCubeMap(0)
		, mUniformId_CubeMap(0)
	{

	}

	cLitShader::~cLitShader()
	{
		if (mProgramId == 0) return;
		glDeleteShader(mProgramId);
		mProgramId = 0;
		mUniformId_ModelMatrix = 0;
		mUniformId_ViewMatrix = 0;
		mUniformId_ProjectionMatrix = 0;
		mUniformId_EyePosition = 0;
		mUniformId_IsCubeMap = 0;
		mUniformId_CubeMap = 0;
		mUniformId_Material = sMaterialUniforms();
		mUniformId_DirLight = sDirLightUniforms();
		mUniformId_SpotLight = sSpotLightUniforms();
		for (int c = 0; c < MAX_POINT_LIGHTS; c++)
		{
			mUniformId_PointLight[c] = sPointLightUniforms();
		}
	}

	bool cLitShader::Load()
	{
		if (IsLoaded()) return true; // all good
		mProgramId = LoadShader("../Assets/shaders/LitShader.vert", "../Assets/shaders/LitShader.frag");
		if (mProgramId == 0)
		{
			std::cout << "shader couldn't load" << std::endl;
			return false;
		}
		// per frame
		mUniformId_ViewMatrix = glGetUniformLocation(mProgramId, "ViewMatrix");
		mUniformId_ProjectionMatrix = glGetUniformLocation(mProgramId, "ProjectionMatrix");
		mUniformId_EyePosition = glGetUniformLocation(mProgramId, "eyePosition");
		// per item
		mUniformId_ModelMatrix = glGetUniformLocation(mProgramId, "ModelMatrix");

		mUniformId_Material.TexDiffuse = glGetUniformLocation(mProgramId, "material.diffuse");
		mUniformId_Material.TexSpecular = glGetUniformLocation(mProgramId, "material.specular");
		mUniformId_Material.Shininess = glGetUniformLocation(mProgramId, "material.shininess");
		mUniformId_Material.MeshColor = glGetUniformLocation(mProgramId, "material.meshColor");

		mUniformId_DirLight.Direction = glGetUniformLocation(mProgramId, "dirLight.direction");
		mUniformId_DirLight.Ambient = glGetUniformLocation(mProgramId, "dirLight.ambient");
		mUniformId_DirLight.Diffuse = glGetUniformLocation(mProgramId, "dirLight.diffuse");
		mUniformId_DirLight.Specular = glGetUniformLocation(mProgramId, "dirLight.specular");

		mUniformId_SpotLight.Position = glGetUniformLocation(mProgramId, "spotLight.position");
		mUniformId_SpotLight.Direction = glGetUniformLocation(mProgramId, "spotLight.direction");
		mUniformId_SpotLight.Ambient = glGetUniformLocation(mProgramId, "spotLight.ambient");
		mUniformId_SpotLight.Diffuse = glGetUniformLocation(mProgramId, "spotLight.diffuse");
		mUniformId_SpotLight.Specular = glGetUniformLocation(mProgramId, "spotLight.specular");
		mUniformId_SpotLight.Constant = glGetUniformLocation(mProgramId, "spotLight.constant");
		mUniformId_SpotLight.Linear = glGetUniformLocation(mProgramId, "spotLight.linear");
		mUniformId_SpotLight.Quadratic = glGetUniformLocation(mProgramId, "spotLight.quadratic");
		mUniformId_SpotLight.CutOff = glGetUniformLocation(mProgramId, "spotLight.cutOff");
		mUniformId_SpotLight.OuterCutOff = glGetUniformLocation(mProgramId, "spotLight.outerCutOff");

		char numbers[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
		std::string pointStringA("pointLights[");
		for (int c = 0; c < MAX_POINT_LIGHTS; c++)
		{
			std::string name = pointStringA + numbers[c] + "].position";
			mUniformId_PointLight[c].Position = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].ambient";
			mUniformId_PointLight[c].Ambient = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].diffuse";
			mUniformId_PointLight[c].Diffuse = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].specular";
			mUniformId_PointLight[c].Specular = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].constant";
			mUniformId_PointLight[c].Constant = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].linear";
			mUniformId_PointLight[c].Linear = glGetUniformLocation(mProgramId, name.c_str());
			name = pointStringA + numbers[c] + "].quadratic";
			mUniformId_PointLight[c].Quadratic = glGetUniformLocation(mProgramId, name.c_str());
		}
		mUniformId_IsCubeMap = glGetUniformLocation(mProgramId, "IsCubeMap");
		mUniformId_CubeMap = glGetUniformLocation(mProgramId, "SamplerCube");
		return true;
	}

	void cLitShader::Bind()
	{
		glUseProgram(mProgramId);
	}
	
	void cLitShader::SetUniforms_PerFrame(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& eyePosition, const sLights& lights)
	{
		SetUniform_ViewMatrix(viewMatrix);
		SetUniform_ProjectionMatrix(projectionMatrix);
		SetUniform_EyePosition(eyePosition);
		SetUniform_DirLight(lights.Directional);
		SetUniform_SpotLight(lights.Spot);
		for (int c = 0; c < MAX_POINT_LIGHTS; c++)
		{
			SetUniform_PointLight(lights.Point[c], c);
		}
	}

	void cLitShader::SetUniforms_PerEntity(const glm::mat4& modelMatrix, const sMaterial& material)
	{
		SetUniform_ModelMatrix(modelMatrix);
		SetUniform_Material(material);
	}
	
}