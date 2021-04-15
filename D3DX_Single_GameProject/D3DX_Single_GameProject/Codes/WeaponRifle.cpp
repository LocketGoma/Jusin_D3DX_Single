#include "framework.h"
#include "WeaponRifle.h"

#include "Transform.h"

#include "ProjPulseAmmo.h"
#include "ProjCoreBall.h"

#include "EffectMuzzle.h"
#include "EffectA2Muzzle.h"

CWeaponRifle::CWeaponRifle(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
	m_iMaxAmmo = 120;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 21;
	m_iMaxMagAmmo = 20;
	m_iAltAmmo = 3;

	m_iROF = 540;
	m_fFireInterval = ONEMINUTE / m_iROF;
	m_fAltFireInterval = 0.6f;

	m_iPriDamage = 3;
	m_iSecDamage = 100;
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

	m_pMeshCom->Set_AnimationSet((_uint)eRifleAction::Idle);

	m_pEffect->Set_EffectPosition(_vec3(210.f, 120.f, 1.f), _vec3(90.f, -80.f, 0.1f));
	m_pAltEffect->Set_EffectPosition(_vec3(180.f, 180.f, 1.f), _vec3(100.f, -75.f, 0.1f));

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

	if (m_pMeshCom->End_AnimationSet())
	{
		Set_Animation((_uint)eRifleAction::Idle);
	}

	if (m_fNowFItime >= m_fFireInterval && m_bFire == true)
	{
		m_bFire = false;
		m_fNowFItime = 0.f;
		m_pEffect->Set_Visible(false);
	}
	if (m_fNowAFItime >= m_fAltFireInterval && m_bAltFire == true)
	{
		m_bAltFire = false;
		m_fNowAFItime = 0.f;
		m_pAltEffect->Set_Visible(false);
	}


	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);

	m_fTime = fDeltaTime;

	
	m_pEffect->LateUpdate_GameObject(fDeltaTime);
	m_pAltEffect->LateUpdate_GameObject(fDeltaTime);

	if (m_fNowAFItime >= 0.1f)
	{
		m_pAltEffect->Set_Visible(false);
	}

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

	if (m_bFire == true)
	{
		m_fNowFItime += m_fTime;
	}
	if (m_bAltFire == true)
	{
		m_fNowAFItime += m_fTime;
	}



	return S_OK;
}

void CWeaponRifle::Draw_Weapon()
{
	Set_Animation((_uint)eRifleAction::Draw);
}

void CWeaponRifle::Shoot_Weapon()
{
	if (m_bFire == false || m_fNowFItime >= m_fFireInterval)
	{
		m_pEffect->Set_Visible(false);
		m_bFire = true;
		if (m_iMagAmmo != 0)
		{
			//-------------------총알 발사파트
			auto pManagement = Engine::CManagement::Get_Instance();
			if (pManagement == nullptr)
			{
				return;
			}

			Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_PulseAmmo");
			NULL_CHECK(pObject);

			_vec3 vPos, vDir;
			_mat matWorld;
			m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			memcpy(&vDir, &matWorld.m[2][0], sizeof(_vec3));
			memcpy(&vPos, &matWorld.m[3][0], sizeof(_vec3));
			D3DXVec3Normalize(&vDir, &vDir);

			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Position(vPos+(vDir*0.5f));
			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_Direction(vDir);
			dynamic_cast<CProjPulseAmmo*>(pObject)->Set_TargetState(eTargetState::ToEnemy);

			TCHAR tObjName[128] = L"";
			TCHAR tObjData[] = L"Ar2Ammo %d";
			swprintf_s(tObjName, tObjData, m_iMagAmmo + m_iMainAmmo);


			if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
			{
				pManagement->Stop_Sound(Engine::SOUND_CHANNELID::EFFECTA);
				pManagement->Play_Sound(L"ar2_fire1.wav", Engine::SOUND_CHANNELID::EFFECTA);

				m_iMagAmmo--;

				Set_Animation((_uint)eRifleAction::Fire);
				m_pEffect->Set_Visible(true);
			}
			else
			{
				Safe_Release(pObject);
			}

			//-----------------------
		}
		m_fNowFItime = 0.f;
	}
	
}

void CWeaponRifle::AltShoot_Weapon()
{
	if (m_bAltFire == false || m_fNowAFItime <= 0.f)
	{
		m_pAltEffect->Set_Visible(false);
		m_bAltFire = true;
		if (m_iAltAmmo != 0)
		{
			auto pManagement = Engine::CManagement::Get_Instance();
			if (pManagement == nullptr)
			{
				return;
			}


			//코어 볼 출력파트
			Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_CoreBall");
			NULL_CHECK(pObject);

			_vec3 vPos, vDir;
			_mat matWorld;
			m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			memcpy(&vDir, &matWorld.m[2][0], sizeof(_vec3));
			memcpy(&vPos, &matWorld.m[3][0], sizeof(_vec3));

			dynamic_cast<CProjCoreBall*>(pObject)->Set_Position(vPos);
			dynamic_cast<CProjCoreBall*>(pObject)->Set_Direction(vDir);

			TCHAR tObjName[128] = L"";
			TCHAR tObjData[] = L"CoreBall %d";
			swprintf_s(tObjName, tObjData, m_iAltAmmo);

			if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
			{
				pManagement->Stop_Sound(Engine::SOUND_CHANNELID::EFFECTA);
				pManagement->Play_Sound(L"ar2_altfire.wav", Engine::SOUND_CHANNELID::EFFECTA);

				m_iAltAmmo--;
				Set_Animation((_uint)eRifleAction::AltFire);
				m_pAltEffect->Set_Visible(true);
			}		
			else
			{
				Safe_Release(pObject);
			}
		}		
	}
}

bool CWeaponRifle::Reload_Weapon()
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return false;
	}

	if (m_iMainAmmo == 0)
		return false;

	if (CPlayerWeapon::Reload_Weapon() == true)
	{		
		m_iMainAmmo--;
		m_iMagAmmo++;
	}
	pManagement->Stop_Sound(Engine::SOUND_CHANNELID::EFFECTA);
	pManagement->Play_Sound(L"ar2_reload.wav", Engine::SOUND_CHANNELID::EFFECTA);
	Set_Animation((_uint)eRifleAction::Reload);

	return true;
}

void CWeaponRifle::Release_Weapon()
{
	Set_Animation((_uint)eRifleAction::Idle);
}

void CWeaponRifle::Change_Weapon()
{

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


	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerRifle"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);


	m_pEffect = dynamic_cast<CEffectMuzzle*>(pManagement->Clone_GameObject(L"Effect_A2_Muzzle"));
	m_pAltEffect = dynamic_cast<CEffectMuzzle*>(pManagement->Clone_GameObject(L"Effect_A2_Muzzle"));

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
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pEffect);
		Safe_Release(m_pAltEffect);
	}

	CPlayerWeapon::Free();
}
