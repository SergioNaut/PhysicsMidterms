#pragma once
#include <extern_includes.h>

// std
#include <string>

namespace nInput
{
	class cInputManager;
	class cKey
	{
	public:
		// helper, for debugging mostly
		std::string Printable;

		inline int GetId() const { return mKeyId; }
		// it is currently down
		inline bool IsDown() const { return mDown; }
		// it is currently up
		inline bool IsUp() const { return !mDown; }
		// has it just changed states between input manager update resets
		inline bool IsJustPressed() const { return mDown && mJust; }
		// has it just changed states between input manager update resets
		inline bool IsJustReleased() const { return !mDown && mJust; }
		// if the button is up, this is how many seconds it has been continously up
		// if the button is down, this is how many seconds it has been continuously down
		inline double GetElapsedTime() { return glfwGetTime() - mTimeStamp; }

	protected:
		cKey(int id);
		virtual ~cKey();

		int mKeyId;
		bool mDown;
		bool mJust;
		double mTimeStamp;

	private:
		friend class cInputManager;
		void Press();
		void Release();

		
		cKey(cKey& other) {}
		cKey& operator=(cKey& other) { return *this; }
	};
}