#include "framework.h"
#include "WeaponShotgun.h"

//디테일작업 나중에 하시오

CWeaponShotgun::CWeaponShotgun(_Device pDevice)
	: CPlayerWeapon(pDevice)
	, m_bReloading(false)
{
	m_iMaxAmmo = 80;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 8;
	m_iMaxMagAmmo = 8;

	m_iROF = 120;
	m_fFireInterval = ONEMINUTE / m_iROF;
	m_fAltFireInterval = 0.8f;

	m_fRecoilPower = 0.075f;

	m_fReloadTime = 0.9f;
	m_fReloadActionTime = 0.0f;

}

CWeaponShotgun::CWeaponShotgun(const CWeaponShotgun& other)
	: CPlayerWeapon(other)
	, m_bReloading(false)
	, m_fReloadTime(other.m_fReloadTime)
{
	m_fReloadActionTime = 0.0f;
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
		m_fReloadActionTime += fDeltaTime;
		Reload_Weapon();
	}

	return NO_EVENT;
}

_int CWeaponShotgun::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	if (m_bFire == true)
	{
		m_fNowFItime += fDeltaTime;
	}

	if (m_pMeshCom->End_AnimationSet())
	{
		if (m_bReloading == false)
		{
			if (m_bFire == true)
			{
				m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Pump);
				m_bFire = false;
			}
			else
			{
				m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Idle);
			}
		}
		else if (m_bReloading == true && m_bEndReload == true)
		{
			m_pMeshCom->Set_AnimationSet((_uint)eShotgunAction::Pump);
			m_bReloading = false;
		}
		else if (m_bReloading = true)
		{
			;
		}
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

_bool CWeaponShotgun::Shoot_Weapon()
{
	_bool bResult = false;

	m_bReloading = false;

	if (m_bFire == false || m_fNowFItime >= m_fFireInterval)
	{
		m_bFire = true;

		if (m_iMagAmmo != 0)
		{
			m_iMagAmmo--;
			Set_Animation((_uint)eShotgunAction::Fire);
			bResult = true;
		}
		else
		{
			Set_Animation((_uint)eShotgunAction::DryFire);
		}
		m_fNowFItime = 0.f;
	}
	return bResult;
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
	//1. 탄창이 가득 차 있을때 or 탄환이 한발도 없을때
	// = 아무것도 안함
	//2. 탄창이 1발이상 비었을때
	//-1.Reload1 모션 재생
	//-2.필요한 탄 수 만큼 Reload2 재생 + 탄 충전
	//-3.모든 탄이 다 들어갔으면 Reload3 재생
	// 
	// 	   아래는 추가 디테일
	//3. 8발 다 쏜 상태에서 장전시 장전 끝나고 Pump
	//4. 장전중 캔슬후 사격 가능

	//if (m_bReloading == false && (m_iMagAmmo == m_iMaxMagAmmo || m_iMainAmmo == 0))
	//	return false;

	//if (m_fReloadActionTime >= m_fReloadTime && eShotgunAction::Reload3 == (eShotgunAction)m_pMeshCom->Get_NowAnimationNumber())
	//{
	//	Set_Animation((_uint)eShotgunAction::Pump);

	//	m_bReloading = false;
	//	m_bEndReload = true;
	//	return false;
	//}

	//if (m_bReloading == false)
	//{
	//	Set_Animation((_uint)eShotgunAction::Reload1);
	//	m_bEndReload = false;
	//	m_bReloading = true;
	//}

	//if (m_bReloading == true && (m_iMagAmmo == m_iMaxMagAmmo || m_iMainAmmo == 0))
	//{
	//	Set_Animation((_uint)eShotgunAction::Reload3);

	//	m_bEndReload = true;
	//	return false;
	//}

	//if (m_fReloadActionTime>=m_fReloadTime && m_pMeshCom->End_AnimationSet())
	//{
	//	Set_Animation((_uint)eShotgunAction::Pump);
	//	Set_Animation((_uint)eShotgunAction::Reload2);

	//	m_iMainAmmo--;
	//	m_iMagAmmo++;

	//	m_fReloadActionTime = 0.f;

	//	m_bReloading = true;
	//}




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

void CWeaponShotgun::Change_Weapon()
{
	;
}


HRESULT CWeaponShotgun::Add_Component(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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
