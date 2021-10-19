#pragma once
#include <extern_includes.h>
#include <graphics/cBuffer.h>
#include <graphics/cMesh.h>
#include <graphics/cVertex.h>
#include <graphics/cMeshManager.h>
#include <graphics/cTextureManager.h>
#include <graphics/shader/cSimpleShader.h>
#include <graphics/shader/cLitShader.h>
#include <graphics/iCamera.h>

namespace nGraphics
{
	extern cTextureManager* gTextureManager;
	extern cMeshManager* gMeshManager;
	extern glm::mat4stack gMatrixStack;
	extern cLitShader* gLitShader;
	extern sLights gLights;
	extern GLFWwindow* gWindow;
	extern iCamera* gCamera;  // to be set by the app

	// helpers, just calls gMatrixStack
	void ClearStack();
	void PushStack(const glm::mat4& modelMatrix);
	void PopStack();

	void SetTempDefaultLights();
	// start up, call it once
	bool Init();
	void SetCamera(iCamera* camera);
	void Callback_WindowResize(GLFWwindow* window, int width, int height);
	void SetWindowTitle(const std::string& title);
	void Focus();
	bool WindowShouldClose();
	float GetTime();
	void BeginFrame(sPerFrameVars& perFrameVars);
	void Render(cMesh* mesh, sPerEntityVars& vars);
	void EndFrame();
	void Shutdown();
}