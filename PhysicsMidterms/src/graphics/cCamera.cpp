#include <graphics/cCamera.h>
#include <input/input.h> // TODO: move input handling somewhere better

namespace nGraphics
{
	sCameraDef::sCameraDef()
	{
		ElasticYaw = false;
		ClampYaw = false;
		ClampPitch = true;
		InvertPitch = false;
		ZoomSpeed = 20.f;
		MinDistance = 2.f;
		Distance = 10.f;
		MaxDistance = 50.f;
		Yaw = 0.f;
		MinYaw = -0.7f;
		MaxYaw = 0.7f;
		YawSpeed = 150.f;
		Pitch = 0.f;
		MinPitch = -0.7f;
		MaxPitch = 0.7f;
		PitchSpeed = 150.f;
		Offset = glm::vec3(0.f, 2.5f, 25.f);
		Offset = glm::normalize(Offset);
	}
	cCamera::cCamera()
	{
		Setup(sCameraDef());
	}

	cCamera::~cCamera()
	{

	}

	void cCamera::Setup(const sCameraDef& def)
	{
		mElasticYaw = def.ElasticYaw;
		mClampYaw = def.ClampYaw;
		mClampPitch = def.ClampPitch;
		mInvertPitch = def.InvertPitch;
		mZoomSpeed = def.ZoomSpeed;
		mMinDistance = def.MinDistance;
		mDistance = def.Distance;
		mMaxDistance = def.MaxDistance;
		mYaw = def.Yaw;
		mMinYaw = def.MinYaw;
		mMaxYaw = def.MaxYaw;
		mYawSpeed = def.YawSpeed;
		mPitch = def.Pitch;
		mMinPitch = def.MinPitch;
		mMaxPitch = def.MaxPitch;
		mPitchSpeed = def.PitchSpeed;
		mOffset = glm::normalize(def.Offset);
	}

	void cCamera::GetProjectionMatrix(glm::mat4& projOut)
	{
		// set up the projection matrix
		projOut = glm::perspective(0.8f, (float)mWidth / (float)mHeight, 0.1f, 1000.f);
	}

	glm::vec3 cCamera::GetForwardAxis()
	{
		return mForwardAxis;
	}

	glm::vec3 cCamera::GetRightAxis()
	{
		return mRightAxis;
	}

	glm::vec3 cCamera::GetUpAxis()
	{
		return mUpAxis;
	}

	void cCamera::GetViewMatrix(glm::mat4& viewOut)
	{
		viewOut = mViewMatrix;
	}

	void cCamera::GetEyePosition(glm::vec4& eyeOut)
	{
		glm::mat4 viewInv((glm::inverse(mViewMatrix)));
		eyeOut.x = viewInv[3].x;
		eyeOut.y = viewInv[3].y;
		eyeOut.z = viewInv[3].z;
		eyeOut.w = 1.f;
	}

	void cCamera::WindowResize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
	}

	void cCamera::Update(float dt)
	{
		float centerX(mWidth / 2);
		float centerY(mHeight / 2);
		double mouseX(0);
		double mouseY(0);
		nInput::GetMousePosition(&mouseX, &mouseY);
		float diffX((float)mouseX - centerX);
		float diffY((float)mouseY - centerY);

		// mouse movement left and right YAW
		float yawSpeed = mYawSpeed * (diffX / mWidth);
		mYaw -= yawSpeed * dt;
		if (mClampYaw)
		{
			mYaw = glm::clamp(mYaw, mMinYaw, mMaxYaw);
		}
		nInput::SetMousePosition(mWidth / 2, mHeight / 2);
		// mouse movement up and down PITCH
		float pitchSpeed = mPitchSpeed * (diffY / mWidth);
		if (mInvertPitch)
		{
			mPitch += pitchSpeed * dt;
		}
		else
		{
			mPitch -= pitchSpeed * dt;
		}
		if (mClampPitch)
		{
			mPitch = glm::clamp(mPitch, mMinPitch, mMaxPitch);
		}
		// UP and DOWN keys for zooming IN and OUT
		bool pressUp = nInput::IsKeyDown::Up();
		bool pressDown = nInput::IsKeyDown::Down();
		if (pressDown != pressUp)
		{
			float zoomSpeed = mZoomSpeed;
			if (pressUp) zoomSpeed *= -1.f;
			mDistance += zoomSpeed * dt;
			if (mDistance < mMinDistance) mDistance = mMinDistance;
			if (mDistance > mMaxDistance) mDistance = mMaxDistance;
		}
	}

	void cCamera::UpdateView()
	{
		glm::vec3 up(0.f, 1.f, 0.f);
		glm::vec3 right(1.f, 0.f, 0.f);

		// set up the view matrix
		// the player's position in global space
		glm::vec3 target(mTargetTranform[3].x, mTargetTranform[3].y, mTargetTranform[3].z);
		// offset, relative to the player's position (w = 0.f so not to get it's translation)
		glm::vec4 offset(mOffset.x, mOffset.y, mOffset.z, 0.f);
		// rotate the player's transform by our yaw amount, then apply it to the offset
		// to get a properly rotated offset, relative to the player's position
		glm::mat4 transform(glm::rotate(mTargetTranform, mYaw, up));
		transform = glm::rotate(transform, mPitch, right);
		offset = transform * offset;
		// extend the offset by the distance we want (zoom)
		offset = glm::normalize(offset) * mDistance;
		// set the eye position to the target's position, plus our perfectly placed offset
		glm::vec3 eye(target.x + offset.x, target.y + offset.y, target.z + offset.z);
		// view!
		mViewMatrix = glm::lookAtRH(eye, target, up);
		// set secondary values
		mForwardAxis = glm::normalize(target - eye);
		mRightAxis = glm::normalize(glm::cross(mForwardAxis, up));
		mUpAxis = glm::normalize(glm::cross(mRightAxis, mForwardAxis));
	}

	void cCamera::SetTargetTransform(glm::mat4& targetTransform)
	{
		mTargetTranform = targetTransform;
	}
}