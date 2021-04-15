#pragma once

#ifndef __COLLIDER_H__
#define __COLLIDER_H__


#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CCollider : public CComponent 
{
private:
	explicit CCollider(_Device pDevice);
	virtual ~CCollider() = default;

public:
	const _matrix* Get_ColMatrix();
	const _vec3* Get_Min();
	const _vec3* Get_Max();

public:
	HRESULT Ready_Collider(const _vec3* pPos, const _ulong& dwVTXCount, const _ulong& dwStride);
	HRESULT Ready_Collider(const _vec3* pPos, const _vec3* vMin, const _vec3* vMax);
	void Render_Collider(COLIDETYPE eType, const _matrix* pColliderMatrix, _bool bIsVisualble);
	void Render_Collider(COLIDETYPE eType, const _vec3* vPos, _bool bIsVisualble);

private:
	_vec3 m_vMin, m_vMax;
	_mat m_matColMatrix;

#ifdef _DEBUG
	_Device			m_pDevice;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DTEXTURE9			m_pTexture[(_uint)COLIDETYPE::COL_END];
#endif
public:
	static CCollider* Create(_Device pDevice, const _vec3* pPos, const _ulong& dwVtxCnt, const _ulong& dwStride);
	static CCollider* Create(_Device pDevice, const _vec3* pPos, const _vec3* vMin, const _vec3* vMax);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void			Free(void);



};

END_NAMESPACE
#endif // !__COLLIDER_H__