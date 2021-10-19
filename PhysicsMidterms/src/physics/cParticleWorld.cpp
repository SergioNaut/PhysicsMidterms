#include <physics/cParticleWorld.h>
#include <algorithm>

namespace nPhysics
{
	cParticleWorld::cParticleWorld()
	{

	}
	cParticleWorld::~cParticleWorld()
	{

	}

	bool cParticleWorld::AddParticle(cParticle* particle)
	{
		// 1) Null check, because you can't add nothing
		if (!particle)
		{
			return false; // nothing was added
		}
		// 2) Check if we have it.
		//    If no: Add it, return true to indicate that we added it.
		//    If yes: Return false, to indicate that nothing was added.
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			// Not there! add it.
			mParticles.push_back(particle);
			return true;
		}
		return false; // nothing was added.
	}

	bool cParticleWorld::RemoveParticle(cParticle* particle)
	{
		// 1) Null check, because you can't remove nothing
		if (!particle)
		{
			return false; // nothing was removed
		}
		// 2) Check if we have it.
		//    If no: Add it, return false to indicate that nothign was removed
		//    If yes: Removed it. Return true to indicate that it was removed
		std::vector<cParticle*>::iterator itParticle = std::find(mParticles.begin(), mParticles.end(), particle);
		if (itParticle == mParticles.end())
		{
			return false; // nothing was removed.
		}
		mParticles.erase(itParticle);
		return true; // found it. removed it.
	}

	void cParticleWorld::TimeStep(float deltaTime)
	{
		// 1) Update Force Generators

		// 2) Integrate the particles
		IntegrateParticles(deltaTime);
		// 3) Generate contacts

		// 4) Resolve contacts

	}

	void cParticleWorld::IntegrateParticles(float deltaTime)
	{
		// loop over our particles and tell them to integrate
		std::vector<cParticle*>::iterator itParticle = mParticles.begin();
		while (itParticle != mParticles.end())
		{
			(*itParticle)->Integrate(deltaTime);
			itParticle++;
		}
	}
}