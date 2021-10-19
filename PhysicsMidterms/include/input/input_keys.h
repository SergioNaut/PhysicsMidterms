#pragma once

#include <extern_includes.h>
#include <graphics/graphics.h>

namespace nInput
{
	namespace KeyCode
	{	
		const int KEY_ESCAPE = GLFW_KEY_ESCAPE;
		const int KEY_SPACE = GLFW_KEY_SPACE;
		const int KEY_1 = GLFW_KEY_1;
		const int KEY_2 = GLFW_KEY_2;
		const int KEY_3 = GLFW_KEY_3;
		const int KEY_4 = GLFW_KEY_4;
		const int KEY_W = GLFW_KEY_W;
		const int KEY_A = GLFW_KEY_A;
		const int KEY_S = GLFW_KEY_S;
		const int KEY_D = GLFW_KEY_D;
		
		
	}

	namespace IsKeyDown
	{
		inline bool __InternalIsKeyDown(int key) { return glfwGetKey(nGraphics::gWindow, key) == GLFW_PRESS; }
		// Space
		inline bool Space() { return __InternalIsKeyDown(GLFW_KEY_SPACE); }
		// Escape
		inline bool Escape() { return __InternalIsKeyDown(GLFW_KEY_ESCAPE); }
		// Letters
		inline bool W() { return __InternalIsKeyDown(GLFW_KEY_W); }
		inline bool A() { return __InternalIsKeyDown(GLFW_KEY_A); }
		inline bool S() { return __InternalIsKeyDown(GLFW_KEY_S); }
		inline bool D() { return __InternalIsKeyDown(GLFW_KEY_D); }
		
		
		// Numbers
		inline bool Num1() { return __InternalIsKeyDown(GLFW_KEY_1); }
		inline bool Num2() { return __InternalIsKeyDown(GLFW_KEY_2); }
		inline bool Num3() { return __InternalIsKeyDown(GLFW_KEY_3); }
		inline bool Num4() { return __InternalIsKeyDown(GLFW_KEY_4); }
	}
	namespace IsKeyUp
	{
		inline bool __InternalIsKeyUp(int key) { return glfwGetKey(nGraphics::gWindow, key) != GLFW_PRESS; }
		inline bool Escape() { return __InternalIsKeyUp(GLFW_KEY_ESCAPE); }
	
	}
	
	//For the Camera
	namespace IsMouseButtonDown
	{
		inline bool __InternalIsMouseButtonDown(int button) { return glfwGetMouseButton(nGraphics::gWindow, button) == GLFW_PRESS; }
		inline bool Left() { return __InternalIsMouseButtonDown(GLFW_MOUSE_BUTTON_LEFT); }
		inline bool Middle() { return __InternalIsMouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE); }
		inline bool Right() { return __InternalIsMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT); }
	}

	namespace IsMouseButtonUp
	{
		inline bool __InternalIsMouseButtonUp(int button) { return glfwGetMouseButton(nGraphics::gWindow, button) != GLFW_PRESS; }
		inline bool Left() { return __InternalIsMouseButtonUp(GLFW_MOUSE_BUTTON_LEFT); }
		inline bool Middle() { return __InternalIsMouseButtonUp(GLFW_MOUSE_BUTTON_MIDDLE); }
		inline bool Right() { return __InternalIsMouseButtonUp(GLFW_MOUSE_BUTTON_RIGHT); }
	}

	inline void GetMousePosition(double* xPos, double* yPos) { glfwGetCursorPos(nGraphics::gWindow, xPos, yPos); }
	inline void SetMousePosition(double xPos, double yPos) { glfwSetCursorPos(nGraphics::gWindow, xPos, yPos); }
}