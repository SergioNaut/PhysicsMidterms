#version 430 core

in vec4 ex_Color;
in vec3 ex_ToEye;
in vec3 ex_Normal;

in vec4 ex_TexCoord;
in vec3 ex_EyePosition;
in vec3 ex_WorldPosition;

uniform vec4 TexFactors;  // x:diffuse, y:??,z:??,w:??
uniform sampler2D TexDiffuse;

// cubemap specific
uniform samplerCube SamplerCube;
uniform bool IsCubeMap;

out vec4 out_Color;

void main()
{
	if (IsCubeMap)
	{
		out_Color = texture(SamplerCube, ex_Normal);
		out_Color.a = 1.0f;
		return;
	}

	out_Color = ex_Color;
	float dot = dot(normalize(ex_ToEye), normalize(ex_Normal)) * 0.7 + 0.3;
	out_Color.x *= dot;
	out_Color.y *= dot;
	out_Color.z *= dot;
	// apply diffuse
	float diffuseFactor = TexFactors.x;
	if (diffuseFactor > 0.0f)
	{
		vec3 diffuse = texture( TexDiffuse, ex_TexCoord.xy ).rgb;
		out_Color.x *= (diffuse.x * diffuseFactor);
		out_Color.y *= (diffuse.y * diffuseFactor);
		out_Color.z *= (diffuse.z * diffuseFactor);
	}
}