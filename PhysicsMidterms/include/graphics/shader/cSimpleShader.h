#pragma once

#include <extern_includes.h>
#include <graphics/cTextureManager.h>
#include <graphics/shader/shader_vars.h>

namespace nGraphics
{
	class cSimpleShader
	{
	public:
		cSimpleShader();
		virtual ~cSimpleShader();
		
		// load it up!
		bool Load();
		// deletes and cleans up stuff
		// dtor will call this
		void Close();
		void Bind();

		// successfully loaded, good to use
		inline bool IsLoaded() const { return mProgramId != 0; }
		inline GLuint GetProgramId() const { return mProgramId; }

		// per-frame
		inline GLuint GetUniform_ViewMatrix() const { return mUniformId_ViewMatrix; }
		inline GLuint GetUniform_PojectionMatrix() const { return mUniformId_ProjectionMatrix; }
		inline GLuint GetUniform_EyePosition() const { return mUniformId_EyePosition; }
		// per-entity
		inline GLuint GetUniform_ModelMatrix() const { return mUniformId_ModelMatrix; }
		inline GLuint GetUniform_ModelColor() const { return mUniformId_ModelColor; }
		inline GLuint GetUniform_TexFactors() const { return mUniformId_TexFactors; }
		inline GLuint GetUniform_TexDiffuse() const { return mUniformId_TexDiffuse; }
		
		
		// per-frame
		inline void SetUniform_ViewMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ViewMatrix, 1, GL_FALSE, m); }
		inline void SetUniform_ProjectionMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ProjectionMatrix, 1, GL_FALSE, m); }
		inline void SetUniform_EyePosition(const float* v) { glUniform4fv(mUniformId_EyePosition, 1, v); }
		
		// per-entity
		inline void SetUniform_IsCubeMap(int b) { glUniform1i(mUniformId_IsCubeMap, b); }
		inline void SetUniform_CubeMap(cTexture* tex) 
		{
			//glActiveTexture(80 + GL_TEXTURE0);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex ? tex->GetId() : -1);
			glUniform1i(mUniformId_CubeMap, 1);
		}
		inline void SetUniform_ModelMatrix(const float* m) { glUniformMatrix4fv(mUniformId_ModelMatrix, 1, GL_FALSE, m); }
		inline void SetUniform_ModelColor(const float* v) { glUniform4fv(mUniformId_ModelColor, 1, v); }
		inline void SetUniform_TexFactors(const float* v) { glUniform4fv(mUniformId_TexFactors, 1, v); }
		inline void SetUniform_TexDiffuse(cTexture* tex)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex ? tex->GetId() : -1);
			glUniform1i(mUniformId_TexDiffuse, 0);
		}
		// per-frame
		inline void SetUniform_ViewMatrix(const glm::mat4& m) { SetUniform_ViewMatrix(glm::value_ptr(m)); }
		inline void SetUniform_ProjectionMatrix(const glm::mat4& m) { SetUniform_ProjectionMatrix(glm::value_ptr(m)); }
		inline void SetUniform_EyePosition(const glm::vec4& v) { SetUniform_EyePosition(glm::value_ptr(v)); }

		// per-entity
		inline void SetUniform_ModelMatrix(const glm::mat4& m) { SetUniform_ModelMatrix(glm::value_ptr(m)); }
		inline void SetUniform_ModelColor(const glm::vec4& v) { SetUniform_ModelColor(glm::value_ptr(v)); }
		inline void SetUniform_TexFactors(const glm::vec4& v) { SetUniform_TexFactors(glm::value_ptr(v)); }

		void SetUniforms_PerFrame(sPerFrameVars& perFrameVars);
		void SetUniforms_PerEntity(sPerEntityVars& perEntityVars);
		void SetUniforms_PerEntity(sPerEntityVars& perEntityVars, const glm::mat4& modelMatrixOverride);
	private:

		GLuint mProgramId;
		GLuint mUniformId_ModelMatrix;
		GLuint mUniformId_ViewMatrix;
		GLuint mUniformId_ProjectionMatrix;
		GLuint mUniformId_ModelColor;
		GLuint mUniformId_EyePosition;
		GLuint mUniformId_TexFactors;
		GLuint mUniformId_TexDiffuse;
		GLuint mUniformId_IsCubeMap;
		GLuint mUniformId_CubeMap;
	};
}