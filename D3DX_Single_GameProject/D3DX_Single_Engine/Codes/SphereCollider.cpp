#include "SphereCollider.h"

USING(Engine);

CSphereCollider::CSphereCollider(_Device pDevice)
	: m_fRadius(0.f)

	, m_pDevice(pDevice)
	 
{
	m_pDevice->AddRef();
}
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

	D3DXCreateSphere(m_pDevice, m_fRadius, 36, 36, &m_pMesh, nullptr);

#ifdef _DEBUG
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

		if (i == (_uint)COLIDETYPE::COL_FALSE)
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		}
		if (i == (_uint)COLIDETYPE::COL_TRUE)
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

		}
		if (i == (_uint)COLIDETYPE::COL_INRANGE)
		{
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		}

		m_pTexture[i]->UnlockRect(0);
	}

#endif // !_DEBUG


	return S_OK;
}
void CSphereCollider::Render_Collider(COLIDETYPE eType, const _matrix* pColliderMatrix, _bool bState)
{
	m_matColMatrix = *pColliderMatrix;

	memcpy(&m_vCore, &m_matColMatrix.m[3][0], sizeof(_vec3));

	m_pDevice->SetTransform(D3DTS_WORLD, &m_matColMatrix);

#ifdef _DEBUG
	if (bState == true)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pDevice->SetTexture(0, m_pTexture[(_uint)eType]);

		m_pMesh->DrawSubset(0);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif
}

void CSphereCollider::Render_Collider(COLIDETYPE eType, const _vec3* vPos, _bool bState)
{
	m_vCore = *vPos;

	_mat matWorld, matPos;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixTranslation(&matPos, m_vCore.x, m_vCore.y, m_vCore.z);

	matWorld *= matPos;

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
#ifdef _DEBUG
	if (bState == true)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pDevice->SetTexture(0, m_pTexture[(_uint)eType]);

		m_pMesh->DrawSubset(0);

		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
#endif

}

const LPD3DXMESH* CSphereCollider::Get_Mesh() const
{
	return &m_pMesh;
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
	Safe_Release(m_pMesh);

	for (_ulong i = 0; i < (_uint)COLIDETYPE::COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pDevice);
}
