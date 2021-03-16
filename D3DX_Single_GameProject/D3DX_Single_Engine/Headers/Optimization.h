#pragma once

#ifndef __OPTIMIZATION_H__
#define __OPTIMIZATION_H__

#include "Component.h"
#include "FrustumCull.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL COptimization : public CComponent
{
private:
	explicit COptimization(_Device pDevice);
	explicit COptimization(const COptimization& other);
	virtual ~COptimization(void) = default;

public:
	//HRESULT				Ready_Optimization(_bool bChoice, const _ulong& dwCntX, const _ulong& dwCntZ);
	HRESULT				Ready_Optimization(const _ulong& dwCntX, const _ulong& dwCntZ);
	_bool				IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius);
	void				IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt);

private:
	_Device					m_pDevice;
	CFrustumCull*			m_pFrustum;


public:
	//static COptimization* Create(_Device pDevice, _bool bChoice, const _ulong& dwCntX, const _ulong& dwCntZ);
	static COptimization* Create(_Device pDevice, const _ulong& dwCntX, const _ulong& dwCntZ);
	virtual CComponent* Clone(void);
	virtual void		Free(void);

};

END_NAMESPACE

#endif // !__OPTIMIZATION_H__