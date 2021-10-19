#pragma once
#include <input/cKey.h>
#include <map>

namespace nInput
{
	// singleton
	class cInputManager
	{
	public:

		static cInputManager* GetInstance();
		~cInputManager();
		inline double GetCursorX() { return mCursorX; }
		inline double GetCursorY() { return mCursorY; }
		inline double GetScrollX() { return mScrollX; }
		inline double GetScrollY() { return mScrollY; }

		cKey* GetKey(int keyId);
		// 
		cKey* ListenToKey(int keyId);
		// call this after everything has had a chance to react to input states
		// to deal with all the JUST-pressed and JUST-released stuff
		void ClearState();

	private:
		static cInputManager* mInstance;
		bool mInitialized;

		typedef std::map<int, cKey*>::iterator IterKey;
		std::map<int, cKey*> mKeys;
		double mCursorX;
		double mCursorY;
		double mScrollX;
		double mScrollY;

		static void KeyCallback(GLFWwindow* window, int keyId, int scancode, int action, int mods);
		static void MouseKeyCallback(GLFWwindow* window, int keyId, int action, int mods);
		static void MousePosCallback(GLFWwindow* window, double x, double y);
		static void ScrollCallback(GLFWwindow* window, double x, double y);

		cInputManager();
		cInputManager(const cInputManager& tm) {}
		cInputManager& operator=(const cInputManager& tm) { return *this; }
	};
}