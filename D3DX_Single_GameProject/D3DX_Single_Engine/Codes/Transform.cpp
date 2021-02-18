#include "Transform.h"

USING(Engine)

CTransform::CTransform()
{
	ZeroMemory(&m_TransformDesc, sizeof(CTransform::TRANSFORM_DESC));

	m_TransformDesc.vScale = _float3(1.f, 1.f, 1.f);
	m_TransformDesc.vRotate = _float3(0.f, 0.f, 0.f);
}

CTransform::CTransform(const CTransform& other)
	: m_TransformDesc(other.m_TransformDesc)	
{
	m_bIsPrototype = false;
}

HRESULT CTransform::Ready_Transform()
{
	m_TransformDesc.m_vInfo[0] = _vec3(0.f, 0.f, 0.f);
	m_TransformDesc.m_vInfo[1] = _vec3(0.f, 0.f, 0.f);
	m_TransformDesc.m_vInfo[2] = _vec3(0.f, 0.f, 0.f);
	m_TransformDesc.m_vInfo[3] = _vec3(0.f, 0.f, 0.f);
	m_TransformDesc.vScale = _float3(1.f, 1.f, 1.f);
	m_TransformDesc.vRotate = _float3(0.f, 0.f, 0.f);
	D3DXMatrixIdentity(&m_TransformDesc.matWorld);

	return S_OK;

}

_int CTransform::Update_Component(const _float& fDeltaTime)
{
	_mat matScale, matRotateX, matRotateY, matRotateZ, matTrans;

	D3DXMatrixScaling(&matScale, m_TransformDesc.vScale.x, m_TransformDesc.vScale.y, m_TransformDesc.vScale.z);
	D3DXMatrixRotationX(&matRotateX, m_TransformDesc.vRotate.x);
	D3DXMatrixRotationY(&matRotateY, m_TransformDesc.vRotate.y);
	D3DXMatrixRotationZ(&matRotateZ, m_TransformDesc.vRotate.z);
	D3DXMatrixTranslation(&matTrans, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].x, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].y, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].z);

	m_TransformDesc.matWorld = matScale * matRotateX * matRotateY * matRotateZ * matTrans;

	//�޸� ä���ֱ�
	for (int i = 0; i < (int)TRANSFORM_INFO::INFO_END; i++)
	{
		memcpy(&m_TransformDesc.m_vInfo[i], &m_TransformDesc.matWorld.m[i][0], sizeof(_vec3));
	}

	return _int();
}

void CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, _float fDeltaTime)
{
	////Right Up Look
	////�� Up����?
	//_vec3 vUp;

	//memcpy(&vUp, &m_TransformDesc.matWorld.m[1][0], sizeof(_vec3));
	//D3DXVec3Normalize(&vUp, &vUp);

	m_TransformDesc.m_vInfo[(_uint)(TRANSFORM_INFO::INFO_POS)] += *pDir * fSpeed * fDeltaTime;
}

void CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	//������ �̿��� �԰�ȭ �� ���� ����.
	*(((_float*)&m_TransformDesc.vRotate) + (_uint)(eType)) += fAngle;
}

//void CTransform::Go_Side(_float fDeltaTime)
//{
//	D3DXVECTOR3 vRight;
//
//	/* ��������� 1���� right�����̴� */
//	memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(D3DXVECTOR3));
//	D3DXVec3Normalize(&vRight, &vRight);
//
//	m_vInfo[(_uint)(TRANSFORM_INFO::INFO_POS)] += vRight * (float)fDeltaTime;
//}

void CTransform::Set_Scale(_vec3 vScale)
{
	m_TransformDesc.vScale = vScale;
}
CTransform::TRANSFORM_DESC CTransform::Get_TransformDescription()
{
	return m_TransformDesc;
}

void CTransform::Get_Info(TRANSFORM_INFO eType, _vec3* pvInfo)
{
	memcpy(pvInfo, &m_TransformDesc.matWorld.m[(_uint)(eType)][0], sizeof(_vec3));

}

_vec3 CTransform::Get_Info(TRANSFORM_INFO eType)
{
	_vec3 vP;

	memcpy(&vP, &m_TransformDesc.matWorld.m[(_uint)(eType)][0], sizeof(_vec3));

	return vP;
}


CTransform* CTransform::Create(_Device pDevice)
{
	return nullptr;
}

CComponent* CTransform::Clone(void* pArg)
{
	return nullptr;
}

void CTransform::Free()
{
}
