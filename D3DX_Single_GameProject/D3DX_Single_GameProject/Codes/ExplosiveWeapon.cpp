#include "framework.h"
#include "ExplosiveWeapon.h"

CExplosiveWeapon::CExplosiveWeapon(_Device pDevice)
	: CBaseProjectile(pDevice)
{
	m_fWeight = 0;
	m_fLifeTime = 2.5f;
	m_fSpeed = 22.5f;
	m_eForceType = eForceType::NONE;
	m_fHitboxSize = 1.5f;

	m_iDamage = 10;
	m_fSplashRadius = 15.f;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;
}

CExplosiveWeapon::CExplosiveWeapon(const CExplosiveWeapon& other)
	: CBaseProjectile(other)
{
}

HRESULT CExplosiveWeapon::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CExplosiveWeapon::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CExplosiveWeapon::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CExplosiveWeapon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CExplosiveWeapon::Render_GameObject(void)
{
	return S_OK;
}

HRESULT CExplosiveWeapon::Add_Component()
{
	return S_OK;
}

CExplosiveWeapon* CExplosiveWeapon::Create(_Device pDevice)
{
	return nullptr;
}

Engine::CGameObject* CExplosiveWeapon::Clone(void* pArg)
{
	return nullptr;
}

void CExplosiveWeapon::Free()
{
}
