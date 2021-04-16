#pragma once

#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__

#include "Component.h"
BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CSphereCollider : public CComponent
{
private:
	explicit CSphereCollider(_Device pDevice);
	virtual ~CSphereCollider() = default;

public:
	const _matrix* Get_ColMatrix();
	const _float Get_Radius();

public:
	HRESULT Ready_Collider(const _vec3* pPos, const _float fRadius);
	HRESULT Ready_Collider(const _vec3* pPos, const _vec3 pScale, const _ulong& dwVTXCount, const _ulong& dwStride, const _float fRadius);
	void Render_Collider(COLIDETYPE eType, const _matrix* pColliderMatrix, _bool bState = true);
	void Render_Collider(COLIDETYPE eType, const _vec3* vPos, _bool bState = true);

	const LPD3DXMESH* Get_Mesh() const;

	const _mat& Get_ColiderMatrix() const;

private:
	_float m_fRadius;
	_vec3 m_vCore;
	_vec3 m_vScale;
	_mat m_matColMatrix;

	_Device			m_pDevice;
	LPD3DXMESH		m_pMesh;
	LPDIRECT3DTEXTURE9	m_pTexture[(_uint)COLIDETYPE::COL_END];

public:
	static CSphereCollider* Create(_Device pDevice, const _vec3* pPos, const _vec3 pScale, const _ulong& dwVtxCnt, const _ulong& dwStride, const _float fRadius);
	static CSphereCollider* Create(_Device pDevice, const _vec3* pPos, const _float fRadius);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void			Free(void);

};

END_NAMESPACE


#endif // !__SPHERE_COLLIDER_H__