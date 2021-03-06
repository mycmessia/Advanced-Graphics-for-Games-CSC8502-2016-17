#pragma once

#include "ParticleEmitter.h"

class VolcanoParticleEmitter : public ParticleEmitter
{
public:
	VolcanoParticleEmitter ();

	virtual void Update (float msec);

protected:
	virtual Particle* GetFreeParticle ();
};