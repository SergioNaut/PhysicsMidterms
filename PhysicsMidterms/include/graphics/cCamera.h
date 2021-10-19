#pragma once
#include <extern_includes.h>

namespace nGraphics
{
	struct sCameraDef
	{
		sCameraDef();
		glm::vec3	Offset;
		float		ZoomSpeed;
		float		Distance;
		float		MaxDistance;
		float		MinDistance;
		float		Yaw;
		float		MinYaw;
		float		MaxYaw;
		float		YawSpeed;
		float		Pitch;
		float		MinPitch;
		float		MaxPitch;
		float		PitchSpeed;
		bool		ElasticYaw;
		bool		ClampYaw;
		bool		ClampPitch;
		bool		InvertPitch;
	};
	class cCamera
	{
	public:
		cCamera();
		~cCamera();
		
		void Setup(const sCameraDef& def);

		glm::vec3 GetForwardAxis();
		glm::vec3 GetRightAxis();
		glm::vec3 GetUpAxis();
		
		void GetProjectionMatrix(glm::mat4& projOut);
		void GetViewMatrix(glm::mat4& viewOut);
		void GetEyePosition(glm::vec4& eyeOut);
		void WindowResize(int width, int height);

		void UpdateView();
		void Update(float dt);
		void SetTargetTransform(glm::mat4& targetTransform);
	private:
		int mWidth;
		int mHeight;
		glm::mat4 mTargetTranform;
		glm::vec3 mForwardAxis; // secondary
		glm::vec3 mRightAxis; // secondary
		glm::vec3 mUpAxis; // secondary
		glm::vec3 mOffset;
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