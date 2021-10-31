#include <graphics/cameras/c3rdPersonCamera.h>
#include <input/input.h>

namespace nGraphics
{

	s3rdPersonCameraDef::s3rdPersonCameraDef()
	{
		ElasticYaw = false;
		ClampYaw = false;
		ClampPitch = true;
		InvertPitch = false;
		ZoomSpeed = 50.f;
		MinDistance = 2.f;
		Distance = 30.f;
		MaxDistance = 100.f;
		Yaw = 0.f;
		MinYaw = -0.7f;
		MaxYaw = 0.7f;
		YawSpeed = 350.f;
		Pitch = 0.f;
		MinPitch = -0.8f;
		MaxPitch = 0.15f;
		PitchSpeed = 350.f;
		Offset = glm::vec3(0.f, 2.5f, 25.f);
		Offset = glm::normalize(Offset);
	}
	c3rdPersonCamera::c3rdPersonCamera()
		: mMouseDown(false)
		, mCurrentZoomSpeed(0.f)
	{
		Setup(s3rdPersonCameraDef());
	}

	c3rdPersonCamera::~c3rdPersonCamera()
	{

	}

	void c3rdPersonCamera::Setup(const s3rdPersonCameraDef& def)
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

	void c3rdPersonCamera::GetProjectionMatrix(glm::mat4& projOut)
	{
		// set up the projection matrix
		projOut = glm::perspective(0.8f, (float)mWidth / (float)mHeight, 0.1f, 1000.f);
	}

	glm::vec3 c3rdPersonCamera::GetForwardAxis()
	{
		return mForwardAxis;
	}

	glm::vec3 c3rdPersonCamera::GetRightAxis()
	{
		return mRightAxis;
	}

	glm::vec3 c3rdPersonCamera::GetUpAxis()
	{
		return mUpAxis;
	}

	void c3rdPersonCamera::GetViewMatrix(glm::mat4& viewOut)
	{
		viewOut = mViewMatrix;
	}

	void c3rdPersonCamera::GetEyePosition(glm::vec4& eyeOut)
	{
		glm::mat4 viewInv((glm::inverse(mViewMatrix)));
		eyeOut.x = viewInv[3].x;
		eyeOut.y = viewInv[3].y;
		eyeOut.z = viewInv[3].z;
		eyeOut.w = 1.f;
	}

	void c3rdPersonCamera::OnWindowResize(int newWidth, int newHeight)
	{
		mWidth = newWidth;
		mHeight = newHeight;
	}

	void c3rdPersonCamera::Update(float dt)
	{
		bool mouseButtonDown = nInput::IsMouseButtonDown::Left();
		if (mouseButtonDown)
		{
			double mouseX = nInput::gInputManager->GetCursorX();
			double mouseY = nInput::gInputManager->GetCursorY();
			if (mMouseDown)
			{
				// not 'just' pressed, use the x,y difference to yaw,pitch
				double diffX(mouseX - mPrevMouseX);
				double diffY(mouseY - mPrevMouseY);

				// mouse movement left and right YAW
				float yawSpeed = mYawSpeed * (diffX / mWidth);
				mYaw -= yawSpeed * dt;
				if (mClampYaw)
				{
					mYaw = glm::clamp(mYaw, mMinYaw, mMaxYaw);
				}
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
			}
			mPrevMouseX = mouseX;
			mPrevMouseY = mouseY;
		}
		mMouseDown = mouseButtonDown;


		// BEGIN
		double scrollY = nInput::gInputManager->GetScrollY();
		bool pressUp = scrollY > 0;
		bool pressDown = scrollY < 0;
		if (pressDown != pressUp)
		{
			mCurrentZoomSpeed = pressDown ? mZoomSpeed : -mZoomSpeed;
		}
		else
		{
			// apply damping until it's close to zero
			// velocity *= pow(1.0 - damping, timeStep);
			mCurrentZoomSpeed *= glm::pow(0.01f, dt);
			if (glm::abs(mCurrentZoomSpeed) < 0.005f)
			{
				mCurrentZoomSpeed = 0.f;
			}
		}
		if (mCurrentZoomSpeed != 0.f)
		{
			mDistance += mCurrentZoomSpeed * dt;
			mDistance = glm::clamp(mDistance, mMinDistance, mMaxDistance);
		}

		UpdateView();
	}

	void c3rdPersonCamera::UpdateView()
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

	void c3rdPersonCamera::SetTargetTransform(glm::mat4& targetTransform)
	{
		mTargetTranform = targetTransform;
	}
}