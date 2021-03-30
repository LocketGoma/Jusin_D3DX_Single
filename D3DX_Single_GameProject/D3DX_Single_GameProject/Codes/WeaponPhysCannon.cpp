#include "framework.h"
#include "WeaponPhysCannon.h"

#include "Transform.h"

CWeaponPhysCannon::CWeaponPhysCannon(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
}

CWeaponPhysCannon::CWeaponPhysCannon(const CWeaponPhysCannon& other)
	: CPlayerWeapon(other)
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
}

void CWeaponPhysCannon::AltShoot_Weapon()
{
}

bool CWeaponPhysCannon::Reload_Weapon()
{
	return false;
}

void CWeaponPhysCannon::Release_Weapon()
{
	Set_Animation((_uint)ePhysAction::Idle);
}

void CWeaponPhysCannon::Holster_Weapon()
{
}

HRESULT CWeaponPhysCannon::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

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

void CWeaponPhysCannon::Free()
{
	CPlayerWeapon::Free();
}
