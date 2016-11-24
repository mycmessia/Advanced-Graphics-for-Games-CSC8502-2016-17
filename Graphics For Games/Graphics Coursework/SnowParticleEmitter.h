#pragma once

#include "ParticleEmitter.h"

class SnowParticleEmitter : public ParticleEmitter 
{
public:
	SnowParticleEmitter ();

	virtual void Update (float msec);

protected:
	virtual Particle* GetFreeParticle ();
};