#pragma once

#include <extern_includes.h>
#include <graphics/cTexture.h>

namespace nGraphics
{
	struct sPerFrameVars
	{
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewMatrix;
		glm::vec4 EyePosition;
	};
	// TODO: rename sPerMeshVars
	struct sPerEntityVars
	{
		sPerEntityVars()
			: TexDiffuse(0)
			, TexFactors(0.f, 0.f, 0.f, 0.f)
			, ModelColor(1.f, 1.f, 1.f, 1.f)
			, IsCubeMap(false)
			, CubeMap(0)
		{

		};
		glm::mat4 ModelMatrix;
		glm::vec4 ModelColor;
		glm::vec4 TexFactors;
		cTexture* TexDiffuse;
		bool IsCubeMap;
		cTexture* CubeMap;
	};

	// JAMES BEGIN
	struct sMaterial
	{
		cTexture* TexDiffuse;
		cTexture* TexSpecular;
		float Shininess;
		glm::vec4 MeshColor;
	};

	struct sDirLight
	{
		glm::vec3 Direction;
		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};

	struct sPointLight
	{
		glm::vec3 Position;

		float Constant;
		float Linear;
		float Quadratic;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};
	const int MAX_POINT_LIGHTS = 4;

	struct sSpotLight {
		glm::vec3 Position;
		glm::vec3 Direction;
		float CutOff;
		float OuterCutOff;

		float Constant;
		float Linear;
		float Quadratic;

		glm::vec3 Ambient;
		glm::vec3 Diffuse;
		glm::vec3 Specular;
	};

	struct sLights
	{
		sDirLight Directional;
		sPointLight Point[MAX_POINT_LIGHTS];
		sSpotLight Spot;
	};
	// JAMES END
}