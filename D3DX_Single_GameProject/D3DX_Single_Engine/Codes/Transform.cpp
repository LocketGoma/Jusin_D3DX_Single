#include "Transform.h"

USING(Engine)

CTransform::CTransform(_Device pDevice)
	:m_pDevice(pDevice)
{
	ZeroMemory(&m_TransformDesc, sizeof(CTransform::TRANSFORM_DESC));
		
	m_TransformDesc.vScale = _float3(1.f, 1.f, 1.f);
	m_TransformDesc.vRotate = _float3(0.f, 0.f, 0.f);

	Safe_AddReference(m_pDevice);
}

CTransform::CTransform(const CTransform& other)
	: m_TransformDesc(other.m_TransformDesc)	
	, m_pDevice(other.m_pDevice)
{
	Safe_AddReference(m_pDevice);

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

	//메모리 채워넣기
	for (int i = 0; i < (int)TRANSFORM_INFO::INFO_END; i++)
	{
		memcpy(&m_TransformDesc.m_vInfo[i], &m_TransformDesc.matWorld.m[i][0], sizeof(_vec3));
	}

	return _int();
}

_int CTransform::Update_Component(_vec3 vAxis, _float fRotate, const _float& fDeltaTime)
{
	_mat matScale, matRotate, matTrans;

	D3DXMatrixScaling(&matScale, m_TransformDesc.vScale.x, m_TransformDesc.vScale.y, m_TransformDesc.vScale.z);
	D3DXMatrixRotationAxis(&matRotate, &vAxis, fRotate);
	D3DXMatrixTranslation(&matTrans, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].x, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].y, m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS].z);

	m_TransformDesc.matWorld = matScale * matRotate * matTrans;

	//메모리 채워넣기
	for (int i = 0; i < (int)TRANSFORM_INFO::INFO_END; i++)
	{
		memcpy(&m_TransformDesc.m_vInfo[i], &m_TransformDesc.matWorld.m[i][0], sizeof(_vec3));
	}

	return _int();
}

_int CTransform::LateUpdate_Component(const _float& fDeltaTime)
{
	if (m_pDevice == nullptr)
	{
		PRINT_LOG(L"Error", L"Transform device Setting Failed");
		return 0;
	}

	return m_pDevice->SetTransform(D3DTS_WORLD, &m_TransformDesc.matWorld);
}

void CTransform::Move_Pos(const _vec3* pDir, const _float& fSpeed, _float fDeltaTime)
{
	////Right Up Look
	////왜 Up이지?
	//_vec3 vUp;

	//memcpy(&vUp, &m_TransformDesc.matWorld.m[1][0], sizeof(_vec3));
	//D3DXVec3Normalize(&vUp, &vUp);

	m_TransformDesc.m_vInfo[(_uint)(TRANSFORM_INFO::INFO_POS)] += *pDir * fSpeed * fDeltaTime;
}

void CTransform::Rotation(ROTATION eType, const _float& fAngle)
{
	//포인터 이용한 규격화 된 간격 연산.
	*(((_float*)&m_TransformDesc.vRotate) + (_uint)(eType)) += fAngle;
}

void CTransform::Rotation(_vec3 vAxis, const _float& fAngle)
{
	_mat matRotate;
	D3DXMatrixRotationAxis(&matRotate, &vAxis, fAngle);

	memcpy(&m_TransformDesc.vRotate.x, &matRotate.m[0][0], sizeof(_vec3));
	memcpy(&m_TransformDesc.vRotate.y, &matRotate.m[1][0], sizeof(_vec3));
	memcpy(&m_TransformDesc.vRotate.z, &matRotate.m[2][0], sizeof(_vec3));

}

void CTransform::Single_Rotation(ROTATION eType, const _float& fAngle)
{
	*(((_float*)&m_TransformDesc.vRotate) + (_uint)(eType)) = fAngle;
}

void CTransform::Reset_Rotation()
{
	_float m_fZero;
	m_fZero = D3DXToRadian(0.f);

	m_TransformDesc.vRotate.x = m_fZero;
	m_TransformDesc.vRotate.y = m_fZero;
	m_TransformDesc.vRotate.z = m_fZero;
}

void CTransform::Set_Pos(_vec3 vPos)
{
	m_TransformDesc.m_vInfo[(_uint)TRANSFORM_INFO::INFO_POS] = vPos;

}

//void CTransform::Go_Side(_float fDeltaTime)
//{
//	D3DXVECTOR3 vRight;
//
//	/* 상태행렬의 1행은 right벡터이다 */
//	memcpy(&vRight, &m_TransformDesc.matWorld.m[0][0], sizeof(D3DXVECTOR3));
//	D3DXVec3Normalize(&vRight, &vRight);
//
//	m_vInfo[(_uint)(TRANSFORM_INFO::INFO_POS)] += vRight * (float)fDeltaTime;
//}

void CTransform::Set_Scale(_vec3 vScale)
{
	m_TransformDesc.vScale = vScale;
}
void CTransform::Set_Info(TRANSFORM_INFO eType, _vec3* pvInfo)
{
	m_TransformDesc.m_vInfo[(_uint)eType] = *pvInfo;
	//memcpy(&m_TransformDesc.matWorld.m[(_uint)(eType)][0], pvInfo, sizeof(_vec3));
}
void CTransform::Set_WorldMatrix(_mat pWorld)
{
	memcpy(m_TransformDesc.matWorld, pWorld, sizeof(_mat));

}
void CTransform::Set_TransformDescription(TRANSFORM_DESC* pDesc)
{
	memcpy(&m_TransformDesc, pDesc, sizeof(TRANSFORM_DESC));
}
CTransform::TRANSFORM_DESC CTransform::Get_TransformDescription() const
{
	return m_TransformDesc;
}

void CTransform::Get_Info(TRANSFORM_INFO eType, _vec3* pvInfo) const
{
	memcpy(pvInfo, &m_TransformDesc.matWorld.m[(_uint)(eType)][0], sizeof(_vec3));	
}

_vec3 CTransform::Get_Info(TRANSFORM_INFO eType) const
{
	_vec3 vP;

	memcpy(&vP, &m_TransformDesc.matWorld.m[(_uint)(eType)][0], sizeof(_vec3));

	return vP;
}

_vec3 CTransform::Get_Info_RawData(TRANSFORM_INFO eType) const
{
	return m_TransformDesc.m_vInfo[(_uint)eType];
}


CTransform* CTransform::Create(_Device pDevice)
{
	CTransform* pIstance = new CTransform(pDevice);
	if (pIstance == nullptr)
	{
		return nullptr;
	}
	pIstance->Ready_Transform();
	

	return pIstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pClone = new CTransform(*this);
	if (FAILED(pClone->Ready_Transform()))
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTransform");
		Safe_Release(pClone);
	}

	return pClone;
}
void CTransform::Free()
{
	Safe_Release(m_pDevice);
}
