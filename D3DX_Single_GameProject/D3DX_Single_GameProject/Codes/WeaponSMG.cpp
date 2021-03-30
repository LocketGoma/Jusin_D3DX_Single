#include "framework.h"
#include "WeaponSMG.h"

#include "Transform.h"

CWeaponSMG::CWeaponSMG(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
	m_iMaxAmmo = 210;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 26;
	m_iMaxMagAmmo = 25;

}

CWeaponSMG::CWeaponSMG(const CWeaponSMG& other)
	: CPlayerWeapon(other)
{
}

HRESULT CWeaponSMG::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponSMG::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));

	m_pMeshCom->Set_AnimationSet((_uint)eSMGAction::Idle);

	return S_OK;
}

_int CWeaponSMG::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);



	return NO_EVENT;
}

_int CWeaponSMG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Set_Scale(_vec3(0.005f, 0.005f, 0.005f));

	m_pTransformCom->Update_Component();

	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CWeaponSMG::Render_GameObject(void)
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
void CWeaponSMG::Draw_Weapon()
{
}

void CWeaponSMG::Shoot_Weapon()
{
	if (m_iMagAmmo != 0)
	{
		m_iMagAmmo--;

		Set_Animation((rand() % 4 + (_uint)eSMGAction::Fire4));
	}

}

void CWeaponSMG::AltShoot_Weapon()
{
	Set_Animation((_uint)eSMGAction::AltFire);
}

bool CWeaponSMG::Reload_Weapon()
{
	if (m_iMainAmmo == 0)
		return false;

	if (CPlayerWeapon::Reload_Weapon() == true)
	{
		m_iMagAmmo++;
	}
	Set_Animation((_uint)eSMGAction::Reload);

	return true;

}

void CWeaponSMG::Release_Weapon()
{
	Set_Animation((_uint)eSMGAction::Idle);
}

void CWeaponSMG::Holster_Weapon()
{
}

HRESULT CWeaponSMG::Add_Component(void)
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
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerSMG"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}

CWeaponSMG* CWeaponSMG::Create(_Device pDevice)
{
	CWeaponSMG* pInstance = new CWeaponSMG(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponSMG::Clone(void* pArg)
{
	CWeaponSMG* pClone = new CWeaponSMG(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponSMG");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponSMG::Free()
{
	CPlayerWeapon::Free();
}

