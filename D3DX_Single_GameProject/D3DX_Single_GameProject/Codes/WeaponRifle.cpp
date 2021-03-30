#include "framework.h"
#include "WeaponRifle.h"

#include "Transform.h"

CWeaponRifle::CWeaponRifle(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
}

CWeaponRifle::CWeaponRifle(const CWeaponRifle& other)
	: CPlayerWeapon(other)
{
}

HRESULT CWeaponRifle::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponRifle::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));

	m_pMeshCom->Set_AnimationSet((_uint)eRifleAction::Draw);

	return S_OK;
}

_int CWeaponRifle::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);



	return NO_EVENT;
}

_int CWeaponRifle::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Set_Scale(_vec3(0.05f, 0.05f, 0.05f));

	m_pTransformCom->Update_Component();

	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CWeaponRifle::Render_GameObject(void)
{
	_mat matView, matScale;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXMatrixScaling(&matScale, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE);
	matView = matScale * matView;
	m_pDevice->SetTransform(D3DTS_WORLD, &matView);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	return S_OK;
}

void CWeaponRifle::Draw_Weapon()
{
}

void CWeaponRifle::Shoot_Weapon()
{
	Set_Animation((_uint)eRifleAction::Fire);
}

void CWeaponRifle::AltShoot_Weapon()
{
	Set_Animation((_uint)eRifleAction::AltFire);
}

void CWeaponRifle::Reload_Weapon()
{
	CPlayerWeapon::Reload_Weapon();

	Set_Animation((_uint)eRifleAction::Reload);
}

void CWeaponRifle::Release_Weapon()
{
	Set_Animation((_uint)eRifleAction::Idle);
}

void CWeaponRifle::Holster_Weapon()
{
	Set_Animation((_uint)eRifleAction::Holster);
}

HRESULT CWeaponRifle::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerRifle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponRifle* CWeaponRifle::Create(_Device pDevice)
{
	CWeaponRifle* pInstance = new CWeaponRifle(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponRifle::Clone(void* pArg)
{
	CWeaponRifle* pClone = new CWeaponRifle(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponRifle");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponRifle::Free()
{
	CPlayerWeapon::Free();
}