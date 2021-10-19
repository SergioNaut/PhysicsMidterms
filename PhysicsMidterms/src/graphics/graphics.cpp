#include <graphics/graphics.h>


// std
#include <iostream>

namespace nGraphics
{
	// globals
	cTextureManager*	gTextureManager = 0;
	cMeshManager*		gMeshManager	= 0;
	cLitShader*			gLitShader		= 0;
	sLights				gLights;
	GLFWwindow*			gWindow			= 0;
	iCamera*            gCamera			= 0;
	glm::mat4stack		gMatrixStack;

	void Focus()
	{
		if (gWindow)
		{
			glfwFocusWindow(gWindow);
		}
	}

	bool WindowShouldClose()
	{
		return glfwWindowShouldClose(gWindow) != 0;
	}
	float GetTime()
	{
		return (float)glfwGetTime();
	}

	// clears color and depth, clears the matrix stack, then sets the per-frame uniform vars
	void BeginFrame(sPerFrameVars& perFrameVars)
	{
		gMatrixStack.clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//gShader->Bind();
		//gShader->SetUniforms_PerFrame(perFrameVars);
		gLitShader->Bind();
		glm::vec3 eyePos(perFrameVars.EyePosition.x, perFrameVars.EyePosition.y, perFrameVars.EyePosition.z);
		gLitShader->SetUniforms_PerFrame(perFrameVars.ViewMatrix, perFrameVars.ProjectionMatrix, eyePos, gLights);
	}

	void ClearStack()
	{
		gMatrixStack.clear();
	}

	void PushStack(const glm::mat4& modelMatrix)
	{
		gMatrixStack.push(modelMatrix);
	}

	void PopStack()
	{
		gMatrixStack.pop();
	}

	void Render(cMesh* mesh, sPerEntityVars& vars)
	{
		if (!mesh) return;
		glm::mat4 modelMatrix = gMatrixStack.getCurrent() * vars.ModelMatrix;
		if (vars.CubeMap)
		{
			gLitShader->SetUniform_IsCubeMap(1);
			gLitShader->SetUniform_CubeMap(vars.CubeMap);
		}
		else
		{
			gLitShader->SetUniform_IsCubeMap(0);
			gLitShader->SetUniform_CubeMap(0);
		}
		//gShader->SetUniforms_PerEntity(vars, modelMatrix);
		sMaterial material;
		material.Shininess = 32.0f;
		material.TexDiffuse = vars.TexDiffuse;
		material.TexSpecular = cTextureManager::GetInstance()->GetTextureByName("whiteSquare");
		material.MeshColor = vars.ModelColor;
		gLitShader->SetUniforms_PerEntity(modelMatrix, material);
		mesh->Bind();
		mesh->Render();
	}

	void EndFrame()
	{
		gMatrixStack.clear();
		// Swap buffers
		glfwSwapBuffers(gWindow);
		glfwPollEvents();
	}

	void SetCamera(iCamera* camera)
	{
		gCamera = camera;
	}

	bool Init()
	{
		if (!glfwInit())
		{
			std::cout << "couldn't initialize glfw :(" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
		// Open a window and create its OpenGL context
		int initWidth = 1024;
		int initHeight = 768;
		gWindow = glfwCreateWindow(initWidth, initHeight, "SuperWindow", NULL, NULL);
		if (gWindow == NULL)
		{
			std::cout << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible." << std::endl;
			glfwTerminate();
			return false;
		}
		glfwMakeContextCurrent(gWindow);

		// so we don't miss the between states for button presses
		glfwSetInputMode(nGraphics::gWindow, GLFW_STICKY_MOUSE_BUTTONS, 1);
		glfwSetInputMode(nGraphics::gWindow, GLFW_STICKY_KEYS, 1);

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEW" << std::endl;
			glfwTerminate();
			gWindow = 0;
			return false;
		}
		// log graphics info
		{
			std::string vendor((char*)glGetString(GL_VENDOR));
			std::string renderer((char*)glGetString(GL_RENDERER));
			std::string version((char*)glGetString(GL_VERSION));
			std::string shadingLanguageVersion((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
			std::cout << "Graphics:" << std::endl;
			std::cout << "\t" << vendor << std::endl;
			std::cout << "\t" << renderer << std::endl;
			std::cout << "\t" << version << std::endl;
			std::cout << "\t" << shadingLanguageVersion << std::endl;
		}

		// create/initialize the mesh manager
		gMeshManager = cMeshManager::GetInstance();

		// create/initialize the texture manager
		gTextureManager = cTextureManager::GetInstance();

		// the camera
		if (gCamera)
		{
			gCamera->OnWindowResize(initWidth, initHeight);
		}

		glfwSetWindowSizeCallback(gWindow, Callback_WindowResize);
		glfwPollEvents();

		// Black background
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);

		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);
		//glCullFace(GL_BACK);
		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);

		// Enable blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Use this to disable VSync (swap as fast as possible!)
		// comment out for VSync
		//glfwSwapInterval(0);

		// load the shader
		/*gShader = new cSimpleShader();
		if (!gShader->Load())
		{
			std::cout << "Shader failed to load" << std::endl;
			delete gShader;
			gShader = 0;
			return false;
		}*/
		// load the awesome new lit shader
		gLitShader = new cLitShader();
		if (!gLitShader->Load())
		{
			std::cout << "Shader failed to load" << std::endl;
			delete gLitShader;
			gLitShader = 0;
			return false;
		}
		SetTempDefaultLights();
		return true;
	}
	
	void SetTempDefaultLights()
	{
		// DIRECTIONAL LIGHTS
		glm::set(gLights.Directional.Direction, -0.2f, -1.0f, -0.3f);
		glm::set(gLights.Directional.Ambient, 0.05f, 0.05f, 0.05f);
		glm::set(gLights.Directional.Diffuse, 0.4f, 0.4f, 0.4f);
		glm::set(gLights.Directional.Specular, 0.5f, 0.5f, 0.5f);
		// POINT LIGHTS
		glm::set(gLights.Point[0].Position, 0.7f, 0.2f, 2.0f);
		glm::set(gLights.Point[1].Position, 2.3f, -3.3f, -4.0f);
		glm::set(gLights.Point[2].Position, -4.0f, 2.0f, -12.0f);
		glm::set(gLights.Point[3].Position, 0.0f, 0.0f, -3.0f);
		for (int c = 0; c < MAX_POINT_LIGHTS; c++)
		{
			glm::set(gLights.Point[c].Ambient, 0.05f, 0.05f, 0.05f);
			glm::set(gLights.Point[c].Diffuse, 0.8f, 0.8f, 0.8f);
			glm::set(gLights.Point[c].Specular, 1.f, 1.f, 1.f);
			gLights.Point[c].Constant = 1.f;
			gLights.Point[c].Linear = 0.09f;
			gLights.Point[c].Quadratic = 0.032f;
		}
		// SPOT LIGHT
		glm::set(gLights.Spot.Position, 0.f, 0.f, -3.f); // try following camera position and
		glm::set(gLights.Spot.Direction, 0.f, 0.f, 1.f); // try following camera direction

		glm::set(gLights.Spot.Ambient, 0.0f, 0.0f, 0.0f);
		glm::set(gLights.Spot.Diffuse, 1.0f, 1.0f, 1.0f);
		glm::set(gLights.Spot.Specular, 1.0f, 1.0f, 1.0f);
		gLights.Spot.Constant = 1.f;
		gLights.Spot.Linear = 0.09f;
		gLights.Spot.Quadratic = 0.032f;
		gLights.Spot.CutOff = glm::cos(glm::radians(12.5f));
		gLights.Spot.OuterCutOff = glm::cos(glm::radians(15.0f));
	}

	void Callback_WindowResize(GLFWwindow* window, int width, int height)
	{
		if (gCamera)
		{
			gCamera->OnWindowResize(width, height);
		}
	}

	void SetWindowTitle(const std::string& title)
	{
		glfwSetWindowTitle(gWindow, title.c_str());
	}

	void Shutdown()
	{
		if (gTextureManager)
		{
			delete gTextureManager;
			gTextureManager = 0;
		}
		if (gLitShader)
		{
			delete gLitShader;
			gLitShader = 0;
		}
		if (gMeshManager)
		{
			gMeshManager->Disable();
			delete gMeshManager;
			gMeshManager = 0;
		}
		glfwTerminate();
		if (gWindow)
		{
			gWindow = 0;
		}
	}
}