#include "SnowParticleEmitter.h"

#include <random>

std::default_random_engine generatorSnow;

SnowParticleEmitter::SnowParticleEmitter ()
{
	texture = SOIL_load_OGL_texture (TEXTUREDIR"snow.png",
									 SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT);

	if (!texture)
	{
		return;
	}
}

void SnowParticleEmitter::Update (float msec)
{
	nextParticleTime -= msec;

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

			++i;
		}
	}

	if (particles.size () > largestSize)
	{
		ResizeArrays ();
	}
}

Particle* SnowParticleEmitter::GetFreeParticle ()
{
	Particle * p = NULL;

	if (freeList.size () > 0)
	{
		p = freeList.back ();
		freeList.pop_back ();
	}
	else
	{
		p = new Particle ();
	}

	std::uniform_real_distribution<float> dis1(0.f, 1.f);
	std::uniform_real_distribution<float> dis2(-1.f, 1.f);
	std::uniform_real_distribution<float> dis3(-1.f, 0.f);

	p->gravity = 2.0f;
	p->colour = Vector4 (
		1.0f, 1.0f, 1.0f, 1.0f
	);

	p->direction = initialDirection;
	p->direction.x += (dis2(generatorSnow) * particleVariance);
	p->direction.y += (dis3(generatorSnow) * particleVariance);
	p->direction.z += (dis2(generatorSnow) * particleVariance);

	p->direction.Normalise ();
	float radius = 5000.0f;
	p->position.x = dis1 (generatorSnow) * radius;
	p->position.y = 3000.0f;
	p->position.z = dis1 (generatorSnow) * radius;

	return p;
}