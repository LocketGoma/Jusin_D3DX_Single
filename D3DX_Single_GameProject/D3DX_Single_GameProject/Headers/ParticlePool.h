#pragma once

#include "MemoryPool.h"
#include "BaseEffect.h"

class CParticlePool : public Engine::CMemoryPool
{
private:
	CParticlePool(CBaseEffect* pEffect);
	virtual ~CParticlePool() = default;



public:
	static CParticlePool* Create(CBaseEffect* pEffect);
	virtual void Free() override;

};

