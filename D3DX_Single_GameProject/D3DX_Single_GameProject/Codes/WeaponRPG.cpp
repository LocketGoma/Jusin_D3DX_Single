#include "framework.h"
#include "WeaponRPG.h"

CWeaponRPG::CWeaponRPG(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
	m_iMaxAmmo = 10;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 1;
	m_iMaxMagAmmo = 1;
}

CWeaponRPG::CWeaponRPG(const CWeaponRPG& other)
	: CPlayerWeapon(other)
{
}

HRESULT CWeaponRPG::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponRPG::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet((_uint)eRPGAction::Draw);

	return S_OK;
}

_int CWeaponRPG::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CWeaponRPG::LateUpdate_GameObject(const _float& fDeltaTime)
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

HRESULT CWeaponRPG::Render_GameObject(void)
{
	_mat matView, matPos, matScale;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXMatrixTranslation(&matPos, 0.f, 0.f, -0.25f);
	D3DXMatrixScaling(&matScale, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE);
	matView = matScale * matPos * matView;
	m_pDevice->SetTransform(D3DTS_WORLD, &matView);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

void CWeaponRPG::Draw_Weapon()
{
	Set_Animation((_uint)eRPGAction::Draw);
}

void CWeaponRPG::Shoot_Weapon()
{
	Set_Animation((_uint)eRPGAction::Fire);
}

void CWeaponRPG::AltShoot_Weapon()
{
}

bool CWeaponRPG::Reload_Weapon()
{
	if (m_iMainAmmo == 0)
		return false;

	if(CPlayerWeapon::Reload_Weapon())
		Set_Animation((_uint)eRPGAction::Reload);

	return true;


}

void CWeaponRPG::Release_Weapon()
{
	Set_Animation((_uint)eRPGAction::Idle);
}

void CWeaponRPG::Holster_Weapon()
{
}

HRESULT CWeaponRPG::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"RocketLauncher"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponRPG* CWeaponRPG::Create(_Device pDevice)
{
	CWeaponRPG* pInstance = new CWeaponRPG(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponRPG::Clone(void* pArg)
{
	CWeaponRPG* pClone = new CWeaponRPG(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponRPG");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponRPG::Free()
{
	CPlayerWeapon::Free();
}
