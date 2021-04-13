#include "framework.h"
#include "WeaponPistol.h"

#include "Transform.h"

#include "EffectMuzzle.h"
#include "ProjBasicAmmo.h"

CWeaponPistol::CWeaponPistol(_Device pDevice)
	: CPlayerWeapon(pDevice)	
{
	m_iMaxAmmo = 210;
	m_iMainAmmo = m_iMaxAmmo;
	m_iMagAmmo = 16;
	m_iMaxMagAmmo = 15;

	m_iROF = 240;
	m_fFireInterval = ONEMINUTE / m_iROF;

	m_iPriDamage = 5;

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

	m_pMeshCom->Set_AnimationSet((_uint)ePistolAction::Idle);

	m_pEffect->Set_EffectPosition(_vec3(100.f, 100.f, 1.f), _vec3(100.f, -100.f, 0.1f));

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

	if (m_pMeshCom->End_AnimationSet())
	{
		if (m_bFire == true)
		{
			if (m_iMagAmmo != 0)
				Set_Animation((_uint)ePistolAction::Idle);
			else
				Set_Animation((_uint)ePistolAction::IdleEmpty);

			m_bFire = false;
		}
	}

	m_pMeshCom->Play_AnimationSet(fDeltaTime);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_TERMINAL_NOALPHA, this);

	m_fTime = fDeltaTime;

	m_pEffect->LateUpdate_GameObject(fDeltaTime);

	return NO_EVENT;
}

HRESULT CWeaponPistol::Render_GameObject(void)
{
	_mat matWorld, matView, matScale;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	D3DXMatrixScaling(&matScale, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE, WEAPON_REDUCION_SIZE);
	matWorld = matScale * matView;
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pMeshCom->Render_Meshes();

	if (m_bFire == true)
	{
		m_fNowFItime += m_fTime;
	}
	if (m_fNowFItime >= 0.1f)
	{
		m_pEffect->Set_Visible(false);
	}

	return S_OK;
}
void CWeaponPistol::Draw_Weapon()
{
}

void CWeaponPistol::Shoot_Weapon()
{

	if (m_bFire == false || m_fNowFItime >= m_fFireInterval)
	{
		m_bFire = true;

		if (m_iMagAmmo != 0)
		{
			//-------------------총알 발사파트
			auto pManagement = Engine::CManagement::Get_Instance();
			if (pManagement == nullptr)
			{
				return;
			}

			Engine::CGameObject* pObject = pManagement->Clone_GameObject(L"Projectile_BasicAmmo");
			NULL_CHECK(pObject);

			_vec3 vPos, vDir;
			_mat matWorld;
			m_pDevice->GetTransform(D3DTS_VIEW, &matWorld);
			D3DXMatrixInverse(&matWorld, NULL, &matWorld);
			memcpy(&vDir, &matWorld.m[2][0], sizeof(_vec3));
			memcpy(&vPos, &matWorld.m[3][0], sizeof(_vec3));
			D3DXVec3Normalize(&vDir, &vDir);

			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_Position(vPos + (vDir * 0.5f));
			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_Direction(vDir);
			dynamic_cast<CProjBasicAmmo*>(pObject)->Set_TargetState(eTargetState::ToEnemy);

			TCHAR tObjName[128] = L"";
			TCHAR tObjData[] = L"PistolAmmo %d";
			swprintf_s(tObjName, tObjData, m_iMagAmmo + m_iMainAmmo);


			if (!FAILED(pManagement->Get_NowScene()->Get_Layer(L"WeaponLayer")->Add_GameObject(tObjName, pObject)))
			{
				m_iMagAmmo--;
				
				Set_Animation(rand() % 4 + (_uint)ePistolAction::Fire3);
				m_pEffect->Set_Visible(true);
			}
			else
			{
				Safe_Release(pObject);
			}
		}
		else
		{
			Set_Animation((_uint)ePistolAction::FireEmpty);
		}

		m_fNowFItime = 0.f;
	}
}

void CWeaponPistol::AltShoot_Weapon()
{
	;
}

bool CWeaponPistol::Reload_Weapon()
{
	if (m_iMainAmmo == 0 || m_iMagAmmo == (m_iMaxMagAmmo+1))
		return false;

	if (CPlayerWeapon::Reload_Weapon() == true)
	{
		m_iMainAmmo--;
		m_iMagAmmo++;
	}
	Set_Animation((_uint)ePistolAction::Reload);

	return true;
}

void CWeaponPistol::Release_Weapon()
{
	if (m_iMagAmmo!=0)
		Set_Animation((_uint)ePistolAction::Idle);
	else
		Set_Animation((_uint)ePistolAction::IdleEmpty);
}

void CWeaponPistol::Holster_Weapon()
{
}

void CWeaponPistol::Change_Weapon()
{
	;
}

HRESULT CWeaponPistol::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// DynamicMesh
	pComponent = m_pMeshCom = dynamic_cast<Engine::CDynamicMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Pistol"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Mesh", pComponent);

	m_pEffect = dynamic_cast<CEffectMuzzle*>(pManagement->Clone_GameObject(L"Effect_Muzzle"));

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
	if (m_bIsPrototype == false)
		Safe_Release(m_pEffect);

	CPlayerWeapon::Free();
}

