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



	return _int();
}

void CTransform::Go_Straight(_float fDeltaTime)
{
}

void CTransform::Go_Side(_float fDeltaTime)
{
}

CTransform::TRANSFORM_DESC CTransform::Get_TransformDescription()
{
	return m_TransformDesc;
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
