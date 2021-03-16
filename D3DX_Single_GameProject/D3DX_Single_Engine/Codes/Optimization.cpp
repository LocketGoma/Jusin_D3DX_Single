#include "Optimization.h"

USING(Engine)

COptimization::COptimization(_Device pDevice)
	: m_pDevice(pDevice)
	, m_pFrustum(nullptr)	
{
	m_pDevice->AddRef();

	m_bIsPrototype = true;
}

COptimization::COptimization(const COptimization& other)
	: m_pDevice(other.m_pDevice)
	, m_pFrustum(other.m_pFrustum)
{
	m_pDevice->AddRef();
	m_bIsPrototype = false;
}

HRESULT COptimization::Ready_Optimization(const _ulong& dwCntX, const _ulong& dwCntZ)
{
	m_pFrustum = CFrustumCull::Create(m_pDevice);
	NULL_CHECK_RETURN(m_pFrustum, E_FAIL);


	return S_OK;
}

_bool COptimization::IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius)
{
	return _bool();
}

void COptimization::IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt)
{
}

COptimization* COptimization::Create(_Device pDevice, const _ulong& dwCntX, const _ulong& dwCntZ)
{
	return nullptr;
}

CComponent* COptimization::Clone(void)
{
	return nullptr;
}

void COptimization::Free(void)
{
}
