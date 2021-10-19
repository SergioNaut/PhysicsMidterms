#include <input/cKey.h>

namespace nInput
{
	cKey::cKey(int id)
		: mKeyId(id), mDown(false), mJust(false)
	{
		mTimeStamp = glfwGetTime();
	}

	cKey::~cKey() { }

	void cKey::Press()
	{
		if (!mDown)
		{
			mDown = true;
			mJust = true;
			mTimeStamp = glfwGetTime();
		}
		else
		{
			// already down
			mJust = false;
		}
	}

	void cKey::Release()
	{
		if (mDown)
		{
			mDown = false;
			mJust = true;
			mTimeStamp = glfwGetTime();
		}
		else
		{
			mJust = false;
		}
	}
}