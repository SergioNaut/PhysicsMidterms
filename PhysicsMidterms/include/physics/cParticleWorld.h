#pragma once
#include <physics/cParticle.h>
#include <physics/cParticleForceRegistry.h>
#include <vector>

namespace nPhysics
{
	class cParticleWorld
	{
	private:
		std::vector<cParticle*> mParticles;
		cParticleForceRegistry* mForceRegistry;
	public:
		cParticleWorld();
		virtual ~cParticleWorld();

		cParticleWorld(cParticleWorld& other) = delete;
		cParticleWorld& operator=(cParticleWorld& other) = delete;

		bool AddParticle(cParticle* particle);
		bool RemoveParticle(cParticle* particle);
		size_t NumParticles();
		void TimeStep(float deltaTime);

		void IntegrateParticles(float deltaTime);

		cParticleForceRegistry* GetForceRegistry() const;

	};
}

