#include "framework.h"
#include "WeaponSMG.h"

#include "EffectMuzzle.h"

#include "ProjBasicAmmo.h"

CWeaponSMG::CWeaponSMG(_Device pDevice)
	: CPlayerWeapon(pDevice)
{
	m_iMaxAmmo = 210;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 26;
	m_iMaxMagAmmo = 25;
	m_iAltAmmo = 5;

	m_iROF = 420;
	m_fFireInterval = ONEMINUTE / m_iROF;
	m_fAltFireInterval = 1.f;

	m_iPriDamage = 2;
	m_iSecDamage = 60;
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


	m_pMeshCom->Set_AnimationSet((_uint)eSMGAction::Idle);

	m_pEffect->Set_EffectPosition(_vec3(150.f, 100.f, 1.f), _vec3(120.f, -80.f, 0.1f));
	m_pAltEffect->Set_EffectPosition(_vec3(180.f, 180.f, 1.f), _vec3(120.f, -75.f, 0.1f));

	return S_OK;
}

_int CWeaponSMG::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CWeaponSMG::LateUpdate_GameObject(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}



	if (m_pMeshCom->End_AnimationSet())
	{
		Set_Animation((_uint)eSMGAction::Idle);
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

	//어쩔수없이 랜더에 처박기...
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
void CWeaponSMG::Draw_Weapon()
{
}

_bool CWeaponSMG::Shoot_Weapon()
{
	_bool bResult = false;

	if (m_bFire == false || m_fNowFItime >= m_fFireInterval)
	{
		m_pEffect->Set_Visible(false);
		m_bFire = true;
		if (m_iMagAmmo != 0)
		{
			//-------------------총알 발사파트
			Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
			if (pManagement == nullptr)
			{
				return false;
			}

			Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_BasicAmmo");
			NULL_CHECK_RETURN(pObject,false);

			//1. 현재 방향, 현재 위치 구함
			//2. 총구 위치 구함
			//3. 총구 위치 -> (현재 방향 x 현재 위치*속도) 값으로 최종 방향 구함
			//4. 3번의 방향, 2번의 위치를 적용시킴.

			_vec3 vPos, vDir, vMuzzlePos, vEndDir;
			_mat matWorld;
			m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			memcpy(&vDir, &matWorld.m[2][0], sizeof(_vec3));
			memcpy(&vPos, &matWorld.m[3][0], sizeof(_vec3));
			D3DXVec3Normalize(&vDir, &vDir);

			vMuzzlePos = _vec3(2.f, -1.5f, 5.f);
			D3DXVec3TransformCoord(&vMuzzlePos, &vMuzzlePos, &matWorld);
			vEndDir = (vPos + vDir * 150.f) - vMuzzlePos;


			D3DXVec3Normalize(&vEndDir, &vEndDir);


			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_Position(vMuzzlePos);
			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_Direction(vEndDir);
			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_TargetState(eTargetState::None);

			TCHAR tObjName[128] = L"";
			TCHAR tObjData[] = L"SMGAmmo %d";
			swprintf_s(tObjName, tObjData, m_iMagAmmo + m_iMainAmmo);


			if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
			{
				pManagement->Stop_Sound(Engine::SOUND_CHANNELID::EFFECTA);
				pManagement->Play_Sound(L"smg1_fire1.wav", Engine::SOUND_CHANNELID::EFFECTA);

				m_iMagAmmo--;

				Set_Animation((rand() % 4 + (_uint)eSMGAction::Fire4));
				m_pEffect->Set_Visible(true);

				bResult = true;
			}
			else
			{
				Safe_Release(pObject);
			}

			//-----------------------
		}
		m_fNowFItime = 0.f;
	}
	return bResult;
}

void CWeaponSMG::AltShoot_Weapon()
{
	if (m_bAltFire == false || m_fNowAFItime >= m_fAltFireInterval)
	{
		m_pAltEffect->Set_Visible(false);
		m_bAltFire = true;
		if (m_iAltAmmo != 0)
		{
			m_iAltAmmo--;		

			Set_Animation((_uint)eSMGAction::AltFire);

			m_pAltEffect->Set_Visible(true);
		}
		m_fNowAFItime = 0.f;
	}

}

bool CWeaponSMG::Reload_Weapon()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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

void CWeaponSMG::Change_Weapon()
{
	;
}

HRESULT CWeaponSMG::Add_Component(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;


	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"PlayerSMG"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	m_pEffect = dynamic_cast<CEffectMuzzle*>(pManagement->Clone_GameObject(L"Effect_Muzzle"));
	m_pAltEffect = dynamic_cast<CEffectMuzzle*>(pManagement->Clone_GameObject(L"Effect_Muzzle"));

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
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pEffect);
		Safe_Release(m_pAltEffect);

	}

	CPlayerWeapon::Free();
}

