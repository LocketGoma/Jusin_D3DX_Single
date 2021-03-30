#include "framework.h"
#include "WeaponShotgun.h"

CWeaponShotgun::CWeaponShotgun(_Device pDevice)
	: CPlayerWeapon(pDevice)
	, m_bReloading(false)
{
	m_iMaxAmmo = 80;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 8;
	m_iMaxMagAmmo = 8;

}

CWeaponShotgun::CWeaponShotgun(const CWeaponShotgun& other)
	: CPlayerWeapon(other)
	, m_bReloading(false)
{
}

HRESULT CWeaponShotgun::Ready_GameObject()
{
	return S_OK;
}

HRESULT CWeaponShotgun::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Draw);

	return S_OK;
}

_int CWeaponShotgun::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	if (m_bReloading == true)
	{
		Reload_Weapon();
	}

	return NO_EVENT;
}

_int CWeaponShotgun::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	if (m_pMeshCom->End_AnimationSet())
	{
		if (m_bFire == true)
		{
			m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Pump);
			m_bFire = false;
		}
		else 
			m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Idle);
	}

	
	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CWeaponShotgun::Render_GameObject(void)
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

void CWeaponShotgun::Draw_Weapon()
{
}

void CWeaponShotgun::Shoot_Weapon()
{
	m_bReloading = false;

	m_bFire = true;

	if (m_iMagAmmo != 0)
	{
		m_iMagAmmo--;
		Set_Animation((_uint)eShotgunAction::Fire);
	}
	else
	{
		Set_Animation((_uint)eShotgunAction::DryFire);
	}
}

void CWeaponShotgun::AltShoot_Weapon()
{
	m_bReloading = false;

	m_bFire = true;

	if (m_iMagAmmo >= 2)
	{
		m_iMagAmmo -= 2;
		Set_Animation((_uint)eShotgunAction::AltFire);
	}
	else
	{
		Set_Animation((_uint)eShotgunAction::DryFire);
	}

}

bool CWeaponShotgun::Reload_Weapon()
{
	if (m_bReloading == false && m_iMainAmmo == 0)
		return false;

	if (m_bReloading == false)
	{
		Set_Animation((_uint)eShotgunAction::Reload1);
	}
	bool test = m_pMeshCom->End_AnimationSet();

	if (m_iMagAmmo == m_iMaxMagAmmo || m_iMainAmmo == 0)
	{
		Set_Animation((_uint)eShotgunAction::Reload3);

	}
	else if (m_bReloading == false || m_pMeshCom->End_AnimationSet())
	{
		Set_Animation((_uint)eShotgunAction::Reload3);
		Set_Animation((_uint)eShotgunAction::Reload2);

		m_iMainAmmo--;
		m_iMagAmmo++;

		m_bReloading = true;

		return true;
	}
	
	if (m_pMeshCom->End_AnimationSet())
	{
		Set_Animation((_uint)eShotgunAction::Pump);

		m_bReloading = false;
		return false;
	}

	//Set_Animation((_uint)eShotgunAction::Reload);

	return true;
}

void CWeaponShotgun::Release_Weapon()
{
	Set_Animation((_uint)eShotgunAction::Idle);
}

void CWeaponShotgun::Holster_Weapon()
{
	m_bReloading = false;


}


HRESULT CWeaponShotgun::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerShotgun"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	return S_OK;
}


CWeaponShotgun* CWeaponShotgun::Create(_Device pDevice)
{
	CWeaponShotgun* pInstance = new CWeaponShotgun(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CWeaponShotgun::Clone(void* pArg)
{
	CWeaponShotgun* pClone = new CWeaponShotgun(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CWeaponShotgun");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CWeaponShotgun::Free()
{
	CPlayerWeapon::Free();
}
