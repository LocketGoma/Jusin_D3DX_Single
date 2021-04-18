#include "framework.h"
#include "ParticlePool.h"

CParticlePool::CParticlePool(CBaseEffect* pEffect)
	: CMemoryPool(pEffect)
{
}

CParticlePool* CParticlePool::Create(CBaseEffect* pEffect)
{
    CParticlePool* pInstance = new CParticlePool(pEffect);

    return pInstance;
}

void CParticlePool::Free()
{
	Engine::CMemoryPool::Free();
}
