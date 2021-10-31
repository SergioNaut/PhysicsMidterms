#include <physics/particle_force_generators.h>

namespace nPhysics
{
	cParticleGravityGenerator::cParticleGravityGenerator(const glm::vec3& gravityAcceleration)
		: mGravityAcceleration(gravityAcceleration)
	{

	}

	cParticleGravityGenerator::~cParticleGravityGenerator()
	{

	}

	void cParticleGravityGenerator::Update(cParticle* particle, float deltaTime)
	{
		// F = M*A
		particle->ApplyForce(mGravityAcceleration * particle->GetMass());
	}
}