#pragma once

#include "ParticleEmitter.h"

class VolcanoParticleEmitter : public ParticleEmitter
{
public:
	virtual void Update (float msec);

protected:
	virtual Particle* GetFreeParticle();
};