#include "FrustumCull.h"

USING (Engine)

CFrustumCull::CFrustumCull(_Device pDevice)
	: m_pDevice(pDevice)
{
	m_pDevice->AddRef();
}

CFrustumCull::CFrustumCull(const CFrustumCull& other)
	: m_pDevice(other.m_pDevice)
{
	for (_uint i = 0; i < 6; ++i)
		m_Plane[i] = other.m_Plane[i];

	for (_uint i = 0; i < 8; ++i)
		m_vPoint[i] = other.m_vPoint[i];

	m_pDevice->AddRef();


}

HRESULT CFrustumCull::Ready_Frustum(void)
{
	//X,Y : -1~1 ���� // Z : 0~1����

	//����
	m_vPoint[0] = _vec3(-1.f, 1.f, 0.f);
	m_vPoint[1] = _vec3(1.f, 1.f, 0.f);
	m_vPoint[2] = _vec3(1.f, -1.f, 0.f);
	m_vPoint[3] = _vec3(-1.f, -1.f, 0.f);

	//�ĸ�
	m_vPoint[4] = _vec3(-1.f, 1.f, 1.f);
	m_vPoint[5] = _vec3(1.f, 1.f, 1.f);
	m_vPoint[6] = _vec3(1.f, -1.f, 1.f);
	m_vPoint[7] = _vec3(-1.f, -1.f, 1.f);

	return S_OK;
}

//������Ʈ ����ü �ø�
_bool CFrustumCull::IsIn_FrustumForObject(const _vec3* pPos, const _float& fRadius)
{
	Update_Frustum();

	return IsIn_Frustum(pPos);
}

_bool CFrustumCull::IsIn_Frustum(const _vec3* pPos)
{
	_float fResult = 0.f;

	//Plane Dot Coord : ���� Ư�� ��ǥ���� ���� ����
	//��� : ��� - ���� / 0 - ���� / ���� - �а�
	//���� : �ٱ�, �а� : ����, ���� : ��ħ

	for (_uint i = 0; i < 6; i++)
	{
		fResult = D3DXPlaneDotCoord(&m_Plane[i], pPos);
		if (fResult > 0.f)
			return false;
	}

	return true;
}

_bool CFrustumCull::IsIn_Frustum(const _vec3* pPos, const _float& fRadius)
{
	//Ư�� ���� ���� ���ϴ��� �˻�
	//����Ʈ����
	_float fResult = 0.f;

	//Plane Dot Coord : ���� Ư�� ��ǥ���� ���� ����
	//��� : ��� - ���� / 0 - ���� / ���� - �а�

	for (_uint i = 0; i < 6; i++)
	{
		fResult = D3DXPlaneDotCoord(&m_Plane[i], pPos);
		if (fResult > fRadius)
			return false;
	}

	return true;
}

void CFrustumCull::IsIn_FrustumForTerrain(const _vec3* pVtxPos, const _ulong& dwCntX, const _ulong& dwCntZ, INDEX32* pIndex, _ulong* pTriCnt)
{
	Update_Frustum();

	_bool	bIsIn[3] = { false };
	_ulong	dwTriCnt = 0;


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;

			// ������ ��
			bIsIn[0] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX]);
			bIsIn[1] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX + 1]);
			bIsIn[2] = IsIn_Frustum(&pVtxPos[dwIndex + 1]);

			if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
			{
				pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
				pIndex[dwTriCnt]._1 = dwIndex + dwCntX + 1;
				pIndex[dwTriCnt]._2 = dwIndex + 1;
				++dwTriCnt;
			}

			// ���� �Ʒ�
			bIsIn[0] = IsIn_Frustum(&pVtxPos[dwIndex + dwCntX]);
			bIsIn[1] = IsIn_Frustum(&pVtxPos[dwIndex + 1]);
			bIsIn[2] = IsIn_Frustum(&pVtxPos[dwIndex]);

			if (true == bIsIn[0] || true == bIsIn[1] || true == bIsIn[2])
			{
				pIndex[dwTriCnt]._0 = dwIndex + dwCntX;
				pIndex[dwTriCnt]._1 = dwIndex + 1;
				pIndex[dwTriCnt]._2 = dwIndex;
				++dwTriCnt;
			}
		}
	}

	*pTriCnt = dwTriCnt;

}

HRESULT CFrustumCull::Update_Frustum()
{
	FAILED_CHECK_RETURN(Ready_Frustum(), E_FAIL);

	_matrix		matProj;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	_matrix		matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_uint i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matProj);
		D3DXVec3TransformCoord(&m_vPoint[i], &m_vPoint[i], &matView);
	}

	// x+
	D3DXPlaneFromPoints(&m_Plane[0], &m_vPoint[1], &m_vPoint[5], &m_vPoint[6]);

	// x-
	D3DXPlaneFromPoints(&m_Plane[1], &m_vPoint[4], &m_vPoint[0], &m_vPoint[3]);

	// y+
	D3DXPlaneFromPoints(&m_Plane[2], &m_vPoint[4], &m_vPoint[5], &m_vPoint[1]);

	// y-
	D3DXPlaneFromPoints(&m_Plane[3], &m_vPoint[3], &m_vPoint[2], &m_vPoint[6]);

	// z+
	D3DXPlaneFromPoints(&m_Plane[4], &m_vPoint[7], &m_vPoint[6], &m_vPoint[5]);

	// z-
	D3DXPlaneFromPoints(&m_Plane[5], &m_vPoint[0], &m_vPoint[1], &m_vPoint[2]);

	return S_OK;
}

CFrustumCull* CFrustumCull::Create(_Device pDeivce)
{
	CFrustumCull* pInstance = new CFrustumCull(pDeivce);

	if (FAILED(pInstance->Ready_Frustum()))
		Safe_Release(pInstance);

	return pInstance;
}

void CFrustumCull::Free()
{
	Safe_Release(m_pDevice);
}
