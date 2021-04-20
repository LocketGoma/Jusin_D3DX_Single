#include "framework.h"
#include "ExplosiveWeapon.h"

#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

#include "ExplosionEffect.h"

CExplosiveWeapon::CExplosiveWeapon(_Device pDevice)
	: CBaseProjectile(pDevice)
	, m_bClearDead(false)
	, m_bExplosion(false)
{
	m_fWeight = 0;
	m_fLifeTime = 2.5f;
	m_fSpeed = 22.5f;
	m_eForceType = eForceType::NONE;
	m_fHitboxSize = 25.f;

	m_iDamage = 10;
	m_fSplashRadius = 15.f;
	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);
	m_fRotate = 0.f;
}

CExplosiveWeapon::CExplosiveWeapon(const CExplosiveWeapon& other)
	: CBaseProjectile(other)
	, m_bClearDead(false)
	, m_bExplosion(true)
{
}

HRESULT CExplosiveWeapon::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CExplosiveWeapon::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CExplosiveWeapon::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	if (m_bExplosion && m_pEffect != nullptr)
	{
		m_pEffect->Set_Position(Get_Position());

		if (m_pEffect->LateUpdate_GameObject(fDeltaTime) == OBJ_DEAD)
		{
			pManagement->Release_Data_To_MemoryPool(L"ExploPool", m_pEffect);
			m_pEffect = nullptr;

			m_bClearDead = true;

			Set_Dead();
		}
	}


	return NO_EVENT;
}

_int CExplosiveWeapon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Update_Component(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_WIREFRAME, this);

	return NO_EVENT;
}

HRESULT CExplosiveWeapon::Render_GameObject(void)
{
	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pTransformCom->LateUpdate_Component(0.f);
	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pColliderCom->Render_Collider(eType, &matWorld, g_bViewCollider);

	if (m_bClearDead == false && m_bDead == true)
	{
		m_iDamage = 0;
		m_bDead = false;
	}

	return S_OK;
}

HRESULT CExplosiveWeapon::Add_Component()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);



	m_pEffect = dynamic_cast<CExplosionEffect*>(pManagement->Get_Data_From_MemoryPool(L"ExploPool"));

	dynamic_cast<CExplosionEffect*>(m_pEffect)->Set_Ready();


	return S_OK;
}

CExplosiveWeapon* CExplosiveWeapon::Create(_Device pDevice)
{
	CExplosiveWeapon* pInstance = new CExplosiveWeapon(pDevice);

	if (FAILED(pInstance->Ready_GameObject(0)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CExplosiveWeapon::Clone(void* pArg)
{
	CExplosiveWeapon* pClone = new CExplosiveWeapon(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CExplosiveWeapon");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CExplosiveWeapon::Free()
{
	if (m_pEffect != nullptr)
	{
		Safe_Release(m_pEffect);
	}

	CBaseProjectile::Free();
}

