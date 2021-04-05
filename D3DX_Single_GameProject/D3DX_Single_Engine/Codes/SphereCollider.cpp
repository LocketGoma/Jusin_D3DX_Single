#include "SphereCollider.h"

USING(Engine);

CSphereCollider::CSphereCollider(_Device pDevice)
	: m_fRadius(0.f)
#ifdef _DEBUG
	, m_pDevice(pDevice)
	 
{
	m_pDevice->AddRef();
}
#else
{
}
#endif // _DEBUG

const _matrix* CSphereCollider::Get_ColMatrix()
{
	return &m_matColMatrix;
}

const _float CSphereCollider::Get_Radius()
{
	return m_fRadius;
}

HRESULT CSphereCollider::Ready_Collider(const _vec3* pPos, const _float fRadius)
{
	m_vCore = *pPos;
	m_fRadius = fRadius;

#ifdef _DEBUG
	D3DXCreateSphere(m_pDevice, m_fRadius, 36, 36, &m_pMesh, nullptr);

	for (_uint i = 0; i < (_uint)COLIDETYPE::COL_END; ++i)
	{
		D3DXCreateTexture(m_pDevice,
			1,
			1,
			1, // miplevel
			0,	// 텍스처의 용도
			D3DFMT_A8R8G8B8,
			D3DPOOL_MANAGED,
			&m_pTexture[i]);


		D3DLOCKED_RECT LockRect;
		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f * i, 1.f * (1 - i), 0.f, 1.f);

		m_pTexture[i]->UnlockRect(0);
	}

#endif // !_DEBUG


	return S_OK;
}
void CSphereCollider::Render_Collider(COLIDETYPE eType, const _matrix* pColliderMatrix)
{
	m_matColMatrix = *pColliderMatrix;

	memcpy(&m_vCore, &m_matColMatrix.m[3][0], sizeof(_vec3));

#ifdef _DEBUG
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matColMatrix);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetTexture(0, m_pTexture[(_uint)eType]);

	m_pMesh->DrawSubset(0);

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

void CSphereCollider::Render_Collider(COLIDETYPE eType, const _vec3* vPos)
{
	m_vCore = *vPos;

	_mat matWorld, matPos;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPos, m_vCore.x, m_vCore.y, m_vCore.z);

	matWorld *= matPos;

#ifdef _DEBUG
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetTexture(0, m_pTexture[(_uint)eType]);

	m_pMesh->DrawSubset(0);

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif

}

CSphereCollider* CSphereCollider::Create(_Device pDevice, const _vec3* pPos, const _float fRadius)
{
	CSphereCollider* pInstance = new CSphereCollider(pDevice);

	if (FAILED(pInstance->Ready_Collider(pPos, fRadius)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CSphereCollider::Clone(void* pArg)
{
	return new CSphereCollider(*this);
}

void CSphereCollider::Free(void)
{
#ifdef _DEBUG
	Safe_Release(m_pMesh);

	for (_ulong i = 0; i < (_uint)COLIDETYPE::COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pDevice);
#endif // _DEBUG


}
