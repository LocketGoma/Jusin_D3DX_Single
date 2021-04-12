#include "framework.h"
#include "WeaponPhysCannon.h"

#include "StaticObject.h"
#include "DynamicObject.h"
#include "BaseProjectile.h"

CWeaponPhysCannon::CWeaponPhysCannon(_Device pDevice)
	: CPlayerWeapon(pDevice)
	, m_pTarget(nullptr)
	, m_pLookTarget(nullptr)
	, m_fGrapGap(5.5f)
	, m_eAction(ePhysAction::Idle)
	, m_bShootLock(true)
{
}

CWeaponPhysCannon::CWeaponPhysCannon(const CWeaponPhysCannon& other)
	: CPlayerWeapon(other)
	, m_pTarget(nullptr)
	, m_pLookTarget(nullptr)
	, m_fGrapGap(other.m_fGrapGap)
	, m_eAction(ePhysAction::Idle)
	, m_bShootLock(true)
{
}

HRESULT CWeaponPhysCannon::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponPhysCannon::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet(2);

	return S_OK;
}

_int CWeaponPhysCannon::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);



	if (m_pTarget != nullptr)
	{
		_mat matView, matPos, matScale;
		m_pDevice->GetTransform(D3DTS_VIEW, &matView);
		D3DXMatrixInverse(&matView, 0, &matView);
		_vec3 vPos = _vec3(0.f,0.f,0.f);
		memcpy(&m_vDir, matView.m[(_uint)Engine::TRANSFORM_INFO::INFO_LOOK], sizeof(_vec3));
		D3DXVec3TransformCoord(&vPos, &vPos, &matView);
		vPos += m_vDir * m_fGrapGap;

		m_pTarget->Set_Position(vPos);
	}


	return NO_EVENT;
}

_int CWeaponPhysCannon::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);
	return NO_EVENT;
}

HRESULT CWeaponPhysCannon::Render_GameObject(void)
{
	_mat matView, matPos, matScale;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXMatrixTranslation(&matPos, 0.f, 0.f, -0.15f);
	D3DXMatrixScaling(&matScale, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE);
	matView = matScale * matPos * matView;
	m_pDevice->SetTransform(D3DTS_WORLD, &matView);

	m_pMeshCom->Set_AnimationSet((_uint)m_eAction);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

void CWeaponPhysCannon::Draw_Weapon()
{
	Set_Animation((_uint)ePhysAction::Draw);
}

void CWeaponPhysCannon::Shoot_Weapon()
{
	if (m_bShootLock == true)
	{
		if (m_pTarget != nullptr)
		{
			m_pTarget->Set_Direction(m_vDir);
			m_pTarget->Set_Speed(15.f);

			m_pTarget = nullptr;
		}
		else if (m_pLookTarget != nullptr)
		{
			m_pLookTarget->Set_Direction(m_vDir);
			m_pLookTarget->Set_Speed(5.f);
		}
		m_bShootLock = false;
	}
}



void CWeaponPhysCannon::AltShoot_Weapon()
{
	//집고있는게 없을때 (잡기)
	if (m_pTarget == nullptr && m_pLookTarget!=nullptr)
	{
		CBaseProjectile* pProj = dynamic_cast<CBaseProjectile*>(m_pLookTarget);
		if (pProj != nullptr)
		{
			pProj->Set_TargetState(eTargetState::ToEnemy);
		}


		m_pTarget = m_pLookTarget;
		m_pTarget->Set_Speed(0.f);
		m_eAction = ePhysAction::Hold_Idle;

	}
	//뭔가 집고 있을때 (놓기)
	else if (m_pTarget != nullptr)
	{
		m_pTarget->Set_Direction(_vec3(0.f, 0.f, 0.f));
		m_pTarget->Set_Speed(0.f);

		m_pTarget = nullptr;		
		m_eAction = ePhysAction::Idle;
	}
	m_pLookTarget = nullptr;
}

bool CWeaponPhysCannon::Reload_Weapon()
{
	return false;
}

void CWeaponPhysCannon::Release_Weapon()
{
	m_bShootLock = true;
	Set_Animation((_uint)ePhysAction::Idle);
}

void CWeaponPhysCannon::Holster_Weapon()
{
}

void CWeaponPhysCannon::Change_Weapon()
{
	m_pLookTarget = nullptr;
	m_pTarget = nullptr;
}

HRESULT CWeaponPhysCannon::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Physcannon"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponPhysCannon* CWeaponPhysCannon::Create(_Device pDevice)
{
	CWeaponPhysCannon* pInstance = new CWeaponPhysCannon(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponPhysCannon::Clone(void* pArg)
{
	CWeaponPhysCannon* pClone = new CWeaponPhysCannon(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponPhysCannon");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponPhysCannon::Set_TargetObject(CBaseObject* pTarget)
{	
	if (m_bShootLock == true)
	{
		m_pLookTarget = pTarget;
	}
	else
	{
		m_pLookTarget = nullptr;
	}
}

CBaseObject* CWeaponPhysCannon::Get_TargetObject()
{
	return m_pTarget;
}

CBaseObject* CWeaponPhysCannon::Get_LookTargetObject()
{
	return m_pLookTarget;
}

void CWeaponPhysCannon::Free()
{
	CPlayerWeapon::Free();
}
