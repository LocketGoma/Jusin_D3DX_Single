#include "Collider.h"
USING(Engine)

CCollider::CCollider(_Device pDevice)
#ifdef _DEBUG
	: m_pDevice(pDevice)
{
	m_pDevice -> AddRef();
}
#else
{
}
#endif // _DEBUG

const _matrix* CCollider::Get_ColMatrix()
{
	return &m_matColMatrix;
}

const _vec3* CCollider::Get_Min()
{
	return &m_vMin;
}

const _vec3* CCollider::Get_Max()
{
	return &m_vMax;
}

HRESULT CCollider::Ready_Collider(const _vec3* pPos, const _ulong& dwVTXCount, const _ulong& dwStride)
{
	D3DXComputeBoundingBox(pPos, dwVTXCount, sizeof(_vec3), &m_vMin, &m_vMax);
#ifdef _DEBUG	
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(sizeof(VTXCUBE) * 8,
		0, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
		FVF_CUBE,
		D3DPOOL_MANAGED, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
		&m_pVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 12,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);


	VTXCUBE* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// 앞면
	pVertex[0].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMin.z);
	pVertex[0].vTexUV = pVertex[0].vPosition;

	pVertex[1].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMin.z);
	pVertex[1].vTexUV = pVertex[1].vPosition;

	pVertex[2].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMin.z);
	pVertex[2].vTexUV = pVertex[2].vPosition;

	pVertex[3].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMin.z);
	pVertex[3].vTexUV = pVertex[3].vPosition;

	// 뒷면
	pVertex[4].vPosition = _vec3(m_vMin.x, m_vMax.y, m_vMax.z);
	pVertex[4].vTexUV = pVertex[4].vPosition;

	pVertex[5].vPosition = _vec3(m_vMax.x, m_vMax.y, m_vMax.z);
	pVertex[5].vTexUV = pVertex[5].vPosition;

	pVertex[6].vPosition = _vec3(m_vMax.x, m_vMin.y, m_vMax.z);
	pVertex[6].vTexUV = pVertex[6].vPosition;

	pVertex[7].vPosition = _vec3(m_vMin.x, m_vMin.y, m_vMax.z);
	pVertex[7].vTexUV = pVertex[7].vPosition;

	m_pVB->Unlock();


	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();


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
#endif
	return S_OK;
}

void CCollider::Render_Collider(COLIDETYPE eType, const _matrix* pColliderMatrix)
{
	m_matColMatrix = *pColliderMatrix;

#ifdef _DEBUG
	m_pDevice->SetTransform(D3DTS_WORLD, pColliderMatrix);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pDevice->SetTexture(0, m_pTexture[(_uint)eType]);

	m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXCUBE));
	m_pDevice->SetFVF(FVF_CUBE);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
}

CCollider* CCollider::Create(_Device pDevice, const _vec3* pPos, const _ulong& dwVtxCnt, const _ulong& dwStride)
{
	CCollider* pInstance = new CCollider(pDevice);

	if (FAILED(pInstance->Ready_Collider(pPos, dwVtxCnt, dwStride)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	return new CCollider(*this);
}

void CCollider::Free(void)
{
#ifdef _DEBUG
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	for (_ulong i = 0; i < (_uint)COLIDETYPE::COL_END; ++i)
		Safe_Release(m_pTexture[i]);

	Safe_Release(m_pDevice);
#endif // _DEBUG
}
