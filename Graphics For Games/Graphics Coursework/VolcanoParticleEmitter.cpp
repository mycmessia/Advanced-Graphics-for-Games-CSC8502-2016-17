#include "VolcanoParticleEmitter.h"

#include <random>

std::default_random_engine generatorVolcano;

void VolcanoParticleEmitter::Update (float msec)
{
	nextParticleTime -= msec;	//some time has passed!

	while (nextParticleTime <= 0)
	{
		nextParticleTime += particleRate;

		for (int i = 0; i < numLaunchParticles; ++i)
		{
			particles.push_back (GetFreeParticle ());
		}
	}

	for (std::vector<Particle *>::iterator i = particles.begin (); i != particles.end ();/*No I++ here!!! */)
	{
		Particle *p = (*i);

		p->colour.w -= (msec / particleLifetime);

		if (p->colour.w <= 0.0f)
		{
			freeList.push_back (p);
			i = particles.erase (i);
		}
		else
		{
			p->position += p->direction*(msec*particleSpeed);
			p->position.y -= (msec * msec * msec * (p->gravity)) / 2000.0f;
			p->gravity += msec * 0.01f;

			++i;
		}
	}

	if (particles.size () > largestSize)
	{
		ResizeArrays ();
	}
}

Particle* VolcanoParticleEmitter:: GetFreeParticle ()
{
	Particle * p = NULL;

	//If we have a spare particle on the freelist, pop it off!
	if (freeList.size () > 0)
	{
		p = freeList.back ();
		freeList.pop_back ();
	}
	else
	{
		//no spare particles, so we need a new one
		p = new Particle ();
	}

	//Now we have to reset its values - if it was popped off the
	//free list, it'll still have the values of its 'previous life'
	std::uniform_real_distribution<float> dis1(0.f, 1.f);
	std::uniform_real_distribution<float> dis2(-1.f, 1.f);

	p->gravity = 2.0f;
	p->colour = Vector4 (1.0f, 1.0f, 1.0f, 1.0f);

	p->direction = initialDirection;
	p->direction.x += (dis2(generatorVolcano) * particleVariance);
	p->direction.y = 1.0f;
	p->direction.z += (dis2(generatorVolcano) * particleVariance);

	p->direction.Normalise ();	//Keep its direction normalised!
	p->position.ToZero ();

	return p;	//return the new particle :-)
}