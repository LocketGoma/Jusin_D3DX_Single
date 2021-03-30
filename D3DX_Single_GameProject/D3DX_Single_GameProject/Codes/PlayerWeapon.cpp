#include "framework.h"
#include "PlayerWeapon.h"

#include "Transform.h"
#include "DynamicMesh.h"

CPlayerWeapon::CPlayerWeapon(_Device pDevice)
	: CGameObject(pDevice)
	, m_pMeshCom(nullptr)
	, m_iMainAmmo(0)
	, m_iAltAmmo(0)
	, m_iMagAmmo(0)
	, m_iMaxMagAmmo(0)
	, m_iMaxAmmo(0)
	, m_iMaxAltAmmo(0)
{
	m_bIsPrototype = true;
}

CPlayerWeapon::CPlayerWeapon(const CPlayerWeapon& other)
	: CGameObject(other)
	, m_pMeshCom(other.m_pMeshCom)
	, m_iMainAmmo(other.m_iMainAmmo)
	, m_iAltAmmo(other.m_iAltAmmo)
	, m_iMaxMagAmmo(other.m_iMaxMagAmmo)
	, m_iMagAmmo(other.m_iMaxMagAmmo)
	, m_iMaxAltAmmo(other.m_iMaxAltAmmo)
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
	if (m_pMeshCom != nullptr)
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

void CPlayerWeapon::Reload_Weapon()
{
	//���� ���� (���� ����)
	m_iMainAmmo -= (m_iMaxMagAmmo - m_iMagAmmo);
	m_iMagAmmo = m_iMaxMagAmmo;
}

_uint CPlayerWeapon::Get_RemainAmmo()
{
	return m_iMainAmmo;
}

_uint CPlayerWeapon::Get_MagAmmo()
{
	return m_iMagAmmo;
}

void CPlayerWeapon::Add_Ammo(_uint _iAmmo)
{
	m_iMainAmmo += _iAmmo;
}

_uint CPlayerWeapon::Get_RemainAltAmmo()
{
	return m_iAltAmmo;
}

void CPlayerWeapon::Add_AltAmmo(_uint _iAmmo)
{
	m_iAltAmmo += _iAmmo;
}
