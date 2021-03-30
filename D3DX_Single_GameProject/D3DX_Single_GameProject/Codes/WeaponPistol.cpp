#include "framework.h"
#include "WeaponPistol.h"

#include "Transform.h"

CWeaponPistol::CWeaponPistol(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
}

CWeaponPistol::CWeaponPistol(const CWeaponPistol& other)
	: CPlayerWeapon(other)
{
}

HRESULT CWeaponPistol::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponPistol::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));

	m_pMeshCom->Set_AnimationSet(2);

	return S_OK;
}

_int CWeaponPistol::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CWeaponPistol::LateUpdate_GameObject(const _float& fDeltaTime)
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

HRESULT CWeaponPistol::Render_GameObject(void)
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
void CWeaponPistol::Draw_Weapon()
{
}

void CWeaponPistol::Shoot_Weapon()
{
	eAction = (ePistolAction)(rand() % 4 + (_uint)ePistolAction::Fire3);
	Set_Animation((_uint)eAction);
}

void CWeaponPistol::AltShoot_Weapon()
{
	;
}

void CWeaponPistol::Reload_Weapon()
{
	Set_Animation((_uint)ePistolAction::Reload);
}

void CWeaponPistol::Release_Weapon()
{
	Set_Animation((_uint)ePistolAction::Idle);
}

void CWeaponPistol::Holster_Weapon()
{
}

HRESULT CWeaponPistol::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Pistol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponPistol* CWeaponPistol::Create(_Device pDevice)
{
	CWeaponPistol* pInstance = new CWeaponPistol(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponPistol::Clone(void* pArg)
{
	CWeaponPistol* pClone = new CWeaponPistol(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponPistol");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponPistol::Free()
{
	CPlayerWeapon::Free();
}

