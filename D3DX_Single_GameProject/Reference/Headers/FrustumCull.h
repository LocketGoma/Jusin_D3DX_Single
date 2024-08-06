#pragma once

#ifndef __FRUSTUM_CULLING_H__
#define __FRUSTUM_CULLING_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CFrustumCull : public CBase
{
private:
	explicit CFrustumCull(_Device pDevice);
	explicit CFrustumCull(const CFrustumCull& other);
	virtual ~CFrustumCull() = default;

public:
	_Device Get_Device() { return m_pDevice; }

public:
	HRESULT				Ready_Frustum(void);
	_bool				IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius);
	_bool				IsIn_Frustum(const _vec3* pPos);
	_bool				IsIn_Frustum(const _vec3* pPos, const _float& fRadius);
	void				IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt);
	HRESULT				Update_Frustum();
	

private:
	_Device				m_pDevice;
	_vec3				m_vPoint[8];//절두체 생성용 8개 점
	D3DXPLANE			m_Plane[6];	//절두체

public:
	static CFrustumCull* Create(_Device pDeivce);
	virtual void Free();

};

END_NAMESPACE

#endif // !__FRUSTUM_CULLING_H__