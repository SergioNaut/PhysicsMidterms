#pragma once

#include <extern_includes.h>
#include <graphics/iCamera.h>

namespace nGraphics
{
	struct s3rdPersonCameraDef
	{
		s3rdPersonCameraDef();
		// This is the initial offset of the eye from the target
		// This will be normalized during setup, use Distance for distance
		glm::vec3	Offset;
		// How fast you wanna zoom?
		float		ZoomSpeed;
		// Initial distance of the eye from the target
		float		Distance;
		// Zoom boundary values
		float		MaxDistance;
		float		MinDistance;
		// Initial left/right pan position. In radians.
		float		Yaw;
		// Yaw boundary values
		float		MinYaw;
		float		MaxYaw;
		// How fast you wanna yaw?  In radians/sec
		float		YawSpeed;
		// Initial up/down tilt position. In radians.
		float		Pitch;
		float		MinPitch;
		float		MaxPitch;
		float		PitchSpeed;
		bool		ElasticYaw;
		bool		ClampYaw;
		bool		ClampPitch;
		bool		InvertPitch;
	};

	class c3rdPersonCamera : public nGraphics::iCamera
	{
	public:
		c3rdPersonCamera();
		virtual ~c3rdPersonCamera();
		// Call this at least once to start
		// Call this each window resize callback
		virtual void OnWindowResize(int newWidth, int newHeight);
		// Call this once to set things up
		void Setup(const s3rdPersonCameraDef& def);

		glm::vec3 GetForwardAxis();
		glm::vec3 GetRightAxis();
		glm::vec3 GetUpAxis();

		void GetProjectionMatrix(glm::mat4& projOut);
		void GetViewMatrix(glm::mat4& viewOut);
		void GetEyePosition(glm::vec4& eyeOut);

		// Call this each physics update
		void Update(float dt);
		// Call this whenever the object you're looking at moves
		// You will be rotating/zooming about this transform
		void SetTargetTransform(glm::mat4& targetTransform);
	private:
		// This is called at the end of Update(float dt) to separate
		// control functionality from the updating functionality
		void UpdateView();
		// Screen width and height
		int mWidth;
		int mHeight;
		// The transform of the object we're looking at
		glm::mat4 mTargetTranform;

		glm::vec3 mForwardAxis; // secondary
		glm::vec3 mRightAxis; // secondary
		glm::vec3 mUpAxis; // secondary

		glm::vec3 mOffset; // generally constant after initial call to Setup()
		double mPrevMouseX;
		double mPrevMouseY;
		bool mMouseDown;
		float mCurrentZoomSpeed;
		float mZoomSpeed;
		float mDistance;
		float mMaxDistance;
		float mMinDistance;
		float mYaw;
		float mMinYaw;
		float mMaxYaw;
		float mYawSpeed;
		float mPitch;
		float mMinPitch;
		float mMaxPitch;
		float mPitchSpeed;
		bool mElasticYaw;
		bool mClampYaw;
		bool mClampPitch;
		bool mInvertPitch;
		glm::mat4 mViewMatrix;
	};
}