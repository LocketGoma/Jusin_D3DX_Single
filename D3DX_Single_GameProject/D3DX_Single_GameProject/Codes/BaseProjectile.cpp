#include "framework.h"
#include "BaseProjectile.h"

CBaseProjectile::CBaseProjectile(_Device pDevice)
	: CStaticObject(pDevice)
	, m_iDamage(0)
	, m_fSplashRadius(0.f)
	, m_fRotate(0.f)
	, m_eTargetState(eTargetState::ToEnemy)
	, m_bAmmoBreak(false)
{
}

CBaseProjectile::CBaseProjectile(const CBaseProjectile& other)
	:CStaticObject(other)
	, m_iDamage(other.m_iDamage)
	, m_fSplashRadius(other.m_fSplashRadius)
	, m_fRotate(other.m_fRotate)
	, m_eTargetState(other.m_eTargetState)
	, m_bAmmoBreak(false)
{
}

const _uint CBaseProjectile::Get_Damage()
{
	return m_iDamage;
}

void CBaseProjectile::Set_Damage(_uint iDamage)
{
	m_iDamage = iDamage;
}



void CBaseProjectile::Set_Radius(_float fRadius)
{
	m_fHitboxSize = fRadius;
}

const _float CBaseProjectile::Get_Splash_Radius()
{
	return m_fSplashRadius;
}

void CBaseProjectile::Set_Splash_Radius(_float fRadius)
{
	m_fSplashRadius = fRadius;
}

const _vec3 CBaseProjectile::Get_StartPos()
{
	return m_vStartPos;
}

void CBaseProjectile::Set_StartPos(_vec3 vPos)
{
	m_vStartPos = vPos;
}

const _vec3 CBaseProjectile::Get_Direction()
{
	return m_vDirection;
}

void CBaseProjectile::Set_Direction(_vec3 vDir)
{
	m_vDirection = vDir;
}

eTargetState CBaseProjectile::Get_TargetState()
{
	return m_eTargetState;
}

void CBaseProjectile::Set_TargetState(eTargetState eState)
{
	m_eTargetState = eState;
}

void CBaseProjectile::Set_Break()
{
	m_bAmmoBreak = true;
}

void CBaseProjectile::Free(void)
{
	CStaticObject::Free();
}

HRESULT CBaseProjectile::Interaction(Engine::CGameObject* pTarget)
{
	return S_OK;
}
