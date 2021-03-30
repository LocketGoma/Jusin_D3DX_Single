#include "framework.h"
#include "PlayerWeapon.h"

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
	, m_bFire(false)
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
	, m_bFire(false)
{

	m_bIsPrototype = false;
	if (m_pMeshCom!=nullptr)
		m_pMeshCom->AddRef();
}

void CPlayerWeapon::Set_Position(_vec3 vPos)
{
	;
}

void CPlayerWeapon::Set_Size(_vec3 vSize)
{
	;
}

_vec3 CPlayerWeapon::Get_Position()
{
	return _vec3();
}

_vec3 CPlayerWeapon::Get_Size()
{
	return _vec3();
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
	}

	Engine::CGameObject::Free();
}

bool CPlayerWeapon::Reload_Weapon()
{
	//장전 공식 (샷건 제외)

	_bool bReturn = false;

	if (m_iMagAmmo == m_iMaxMagAmmo + 1)
	{
		return false;
	}

	if (m_iMagAmmo > 0)
	{
		bReturn = true;
	}
	if ((m_iMainAmmo + m_iMagAmmo) > m_iMaxMagAmmo)
	{
		m_iMainAmmo -= (m_iMaxMagAmmo - m_iMagAmmo);
		m_iMagAmmo = m_iMaxMagAmmo;
	}
	else
	{
		m_iMagAmmo = (m_iMainAmmo + m_iMagAmmo);
		m_iMainAmmo = 0;
	}


	return bReturn;
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
