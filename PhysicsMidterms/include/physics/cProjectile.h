#pragma once
#include <physics/cParticle.h>

namespace nPhysics
{
	struct sProjectileDef
	{
		float Mass;
		float Damping;
		glm::vec3 Position;
		float Speed;
		glm::vec3 Direction;
		glm::vec3 Acceleration;
		float EnergyAcceleration;
		float Radius;
		glm::vec4 Colour;
		float GroundHeight;
	};

	class cProjectile : public cParticle
	{
	private:
		float mRadius;
		glm::vec4 mColour;
		float mGroundHeight;
		float mEnergyAcceleration;
	public:
		cProjectile(const sProjectileDef& def);
		virtual ~cProjectile();

		cProjectile() = delete;
		cProjectile(cProjectile& other) = delete;
		cProjectile& operator=(cProjectile& other) = delete;

		float GetRadius();
		const glm::vec4& GetColour();

		virtual void Integrate(float deltaTime);
	};
}