#include "framework.h"
#include "PlayerWeapon.h"

#include "Transform.h"
#include "DynamicMesh.h"

CPlayerWeapon::CPlayerWeapon(_Device pDevice)
	: CGameObject(pDevice)
	, m_pMeshCom(nullptr)	
{
	m_bIsPrototype = true;
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon& other)
	: CGameObject(other)
	, m_pMeshCom(other.m_pMeshCom)

{

	m_bIsPrototype = false;
	if (m_pMeshCom!=nullptr)
		m_pMeshCom->AddRef();
}

void CPlayerWeapon::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CPlayerWeapon::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CPlayerWeapon::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CPlayerWeapon::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

void CPlayerWeapon::Set_Animation(int iNumber)
{
	m_pMeshCom->Set_AnimationSet(iNumber);
}

int CPlayerWeapon::Get_VertexNumber()
{
	return m_pMeshCom->Get_VertexCount();
}

void CPlayerWeapon::Free()
{
	if (m_bIsPrototype == true)
	{
		Safe_Release(m_pMeshCom);
		Safe_Release(m_pTransformCom);
	}

	Engine::CGameObject::Free();
}
