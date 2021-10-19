#pragma once
#include <physics/cParticle.h>

#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
	public:
		cParticleWorld();
		virtual ~cParticleWorld();

		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);

		void TimeStep(float deltaTime);

		void IntegrateParticles(float deltaTime);
	};
}

