#include <physics/cProjectile.h>


namespace nPhysics
{
	cProjectile::cProjectile(const sProjectileDef& def)
		: cParticle(def.Mass, def.Position)
		, mRadius(def.Radius)
		, mColour(def.Colour)
		, mGroundHeight(def.GroundHeight)
		, mEnergyAcceleration(def.EnergyAcceleration)
	{
		SetVelocity(def.Speed * glm::normalize(def.Direction));
		SetAcceleration(def.Acceleration);
		SetDamping(def.Damping);

	}
	cProjectile::~cProjectile()
	{

	}

	float cProjectile::GetRadius()
	{
		return mRadius;
	}
	const glm::vec4& cProjectile::GetColour()
	{
		return mColour;
	}

	void cProjectile::Integrate(float deltaTime)
	{
		if (!GetIsAlive())
		{
			return;
		}
		ApplyForce(glm::normalize(GetVelocity()) * mEnergyAcceleration * GetMass());
		cParticle::Integrate(deltaTime);
		if (GetPosition().y < mGroundHeight + mRadius)
		{
			SetIsAlive(false);
		}
	}
}