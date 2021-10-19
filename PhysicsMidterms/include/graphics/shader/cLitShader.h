#pragma once
#include <extern_includes.h>
#include <graphics/cTextureManager.h>
#include <graphics/shader/shader_vars.h>

namespace nGraphics
{
	class cLitShader
	{
		struct sMaterialUniforms
		{
			sMaterialUniforms();
			//texture
			GLuint TexDiffuse;
			GLuint TexSpecular;
			//float
			GLuint Shininess;
			GLuint MeshColor;
		};
		struct sDirLightUniforms
		{
			sDirLightUniforms();
			//vec3
			GLuint Direction;
			GLuint Ambient;
			GLuint Diffuse;
			GLuint Specular;
		};
		struct sPointLightUniforms
		{
			sPointLightUniforms();
			//vec3
			GLuint Position;
			//float
			GLuint Constant;
			GLuint Linear;
			GLuint Quadratic;
			//vec3
			GLuint Ambient;
			GLuint Diffuse;
			GLuint Specular;
		};
		struct sSpotLightUniforms
		{
			sSpotLightUniforms();
			//vec3
			GLuint Position;
			GLuint Direction;
			//float
			GLuint CutOff;
			GLuint OuterCutOff;
			GLuint Constant;
			GLuint Linear;
			GLuint Quadratic;
			//vec3
			GLuint Ambient;
			GLuint Diffuse;
			GLuint Specular;
		};
	public:
		cLitShader();
		virtual ~cLitShader();

		// load it up!
		bool Load();
		void Bind();

		// successfully loaded, good to use
		inline bool IsLoaded() const { return mProgramId != 0; }
		inline GLuint GetProgramId() const { return mProgramId; }

		// per-frame
		inline void SetUniform_ViewMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ViewMatrix, 1, GL_FALSE, m); }
		inline void SetUniform_ProjectionMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ProjectionMatrix, 1, GL_FALSE, m); }
		inline void SetUniform_EyePosition(const float* v) { glUniform3fv(mUniformId_EyePosition, 1, v); }
		inline void SetUniform_ViewMatrix(const glm::mat4& m) { SetUniform_ViewMatrix(glm::value_ptr(m)); }
		inline void SetUniform_ProjectionMatrix(const glm::mat4& m) { SetUniform_ProjectionMatrix(glm::value_ptr(m)); }
		inline void SetUniform_EyePosition(const glm::vec3& v) { SetUniform_EyePosition(glm::value_ptr(v)); }
		inline void SetUniform_DirLight(const sDirLight& dirLight)
		{
			glUniform3fv(mUniformId_DirLight.Direction, 1, glm::value_ptr(dirLight.Direction));
			glUniform3fv(mUniformId_DirLight.Ambient, 1, glm::value_ptr(dirLight.Ambient));
			glUniform3fv(mUniformId_DirLight.Diffuse, 1, glm::value_ptr(dirLight.Diffuse));
			glUniform3fv(mUniformId_DirLight.Specular, 1, glm::value_ptr(dirLight.Specular));
		}
		inline void SetUniform_SpotLight(const sSpotLight& spotLight)
		{
			glUniform3fv(mUniformId_SpotLight.Position, 1, glm::value_ptr(spotLight.Position));
			glUniform3fv(mUniformId_SpotLight.Direction, 1, glm::value_ptr(spotLight.Direction));
			glUniform1f(mUniformId_SpotLight.CutOff, spotLight.CutOff);
			glUniform1f(mUniformId_SpotLight.OuterCutOff, spotLight.OuterCutOff);
			glUniform1f(mUniformId_SpotLight.Constant, spotLight.Constant);
			glUniform1f(mUniformId_SpotLight.Linear, spotLight.Linear);
			glUniform1f(mUniformId_SpotLight.Quadratic, spotLight.Quadratic);
			glUniform3fv(mUniformId_SpotLight.Ambient, 1, glm::value_ptr(spotLight.Ambient));
			glUniform3fv(mUniformId_SpotLight.Diffuse, 1, glm::value_ptr(spotLight.Diffuse));
			glUniform3fv(mUniformId_SpotLight.Specular, 1, glm::value_ptr(spotLight.Specular));
		}
		inline void SetUniform_PointLight(const sPointLight& pointLight, int index)
		{
			sPointLightUniforms& uniforms = mUniformId_PointLight[index];
			glUniform3fv(uniforms.Position, 1, glm::value_ptr(pointLight.Position));
			glUniform1f(uniforms.Constant, pointLight.Constant);
			glUniform1f(uniforms.Linear, pointLight.Linear);
			glUniform1f(uniforms.Quadratic, pointLight.Quadratic);
			glUniform3fv(uniforms.Ambient, 1, glm::value_ptr(pointLight.Ambient));
			glUniform3fv(uniforms.Diffuse, 1, glm::value_ptr(pointLight.Diffuse));
			glUniform3fv(uniforms.Specular, 1, glm::value_ptr(pointLight.Specular));
		}

		// per-entity
		inline void SetUniform_ModelMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ModelMatrix, 1, GL_FALSE, m); }
		
		inline void SetUniform_Material(const sMaterial& material)
		{
			glUniform1i(mUniformId_Material.TexDiffuse, 0);
			glUniform1i(mUniformId_Material.TexSpecular, 1);
			glUniform1f(mUniformId_Material.Shininess, material.Shininess);
			glUniform4fv(mUniformId_Material.MeshColor, 1,  glm::value_ptr(material.MeshColor));
			// bind diffuse map
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material.TexDiffuse ? material.TexDiffuse->GetId() : -1);
			// bind specular map
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, material.TexSpecular ? material.TexSpecular->GetId() : -1);
		}

		// per-entity
		inline void SetUniform_IsCubeMap(int b) { glUniform1i(mUniformId_IsCubeMap, b); }
		inline void SetUniform_CubeMap(cTexture* tex)
		{
			//glActiveTexture(80 + GL_TEXTURE0);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, tex ? tex->GetId() : -1);
			glUniform1i(mUniformId_CubeMap, 2);
		}
		inline void SetUniform_ModelMatrix(const glm::mat4& m) { SetUniform_ModelMatrix(glm::value_ptr(m)); }

		void SetUniforms_PerFrame(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& eyePosition, const sLights& lights);
		void SetUniforms_PerEntity(const glm::mat4& modelMatrix, const sMaterial& material);
	private:

		GLuint mProgramId;

		GLuint mUniformId_ModelMatrix;
		GLuint mUniformId_ViewMatrix;
		GLuint mUniformId_ProjectionMatrix;

		GLuint mUniformId_EyePosition;
		sMaterialUniforms mUniformId_Material;
		sDirLightUniforms mUniformId_DirLight;
		sPointLightUniforms mUniformId_PointLight[MAX_POINT_LIGHTS];
		sSpotLightUniforms mUniformId_SpotLight;
		
		GLuint mUniformId_IsCubeMap;
		GLuint mUniformId_CubeMap;
	};
}