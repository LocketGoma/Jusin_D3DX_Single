#include "framework.h"
#include "ProjPulseAmmo.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

////쓸 수 있는 인자들
///from StaticObject:
//_uint m_iWeight;
//_float m_fLifeTime;
//_float m_fSpeed;
//eForceType m_eForceType;
// _float m_fHitboxSize;
//
//Engine::CStaticMesh* m_pMeshCom;
//Engine::CTransform* m_pTransformCom;
//Engine::CControlSupportUnit* m_pSupportCom;
//Engine::CSphereCollider* m_pColliderCom;

///from BaseProjectile:
//_uint m_iDamage;
//_float m_fSplashRadius;
//_vec3 m_vStartPos;
//_vec3 m_vDirection;
//_float m_fRotate;

CProjPulseAmmo::CProjPulseAmmo(_Device pDevice)
	: CBaseProjectile(pDevice)
{
}

CProjPulseAmmo::CProjPulseAmmo(const CProjPulseAmmo& other)
	: CBaseProjectile(other)
{
}

HRESULT CProjPulseAmmo::Ready_GameObject(_uint iTexNumber = 0)
{
	return E_NOTIMPL;
}

HRESULT CProjPulseAmmo::Ready_GameObject_Clone(void* pArg)
{
	return E_NOTIMPL;
}

_int CProjPulseAmmo::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CProjPulseAmmo::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CProjPulseAmmo::Render_GameObject(void)
{
	return E_NOTIMPL;
}

HRESULT CProjPulseAmmo::Add_Component()
{
	return E_NOTIMPL;
}

CProjPulseAmmo* CProjPulseAmmo::Create(_Device pDevice)
{
	return nullptr;
}

CGameObject* CProjPulseAmmo::Clone(void* pArg = nullptr)
{
	return nullptr;
}

void CProjPulseAmmo::Free()
{
}
