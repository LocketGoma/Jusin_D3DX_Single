#include "framework.h"
#include "Player.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "ControlSupport.h"

#include "CameraComponent.h"

//무기

#include "PlayerWeapon.h"
#include "WeaponCrowBar.h"

float CPlayer::g_zDelta = 0.f;

CPlayer::CPlayer(_Device pDevice)
	: Engine::CGameObject(pDevice)
	, m_pWeaponType(eWeaponType::WEAPON_CROWBAR)
	, m_fWeaponTimer(0.f)
	, m_bShootState(false)
{
	ZeroMemory(&m_pWeapon, sizeof(void*) * (_uint)eWeaponType::WEAPON_END);
}

CPlayer::CPlayer(const CPlayer& other)
	: Engine::CGameObject(other)
	, m_pWeaponType(other.m_pWeaponType)
	, m_fWeaponTimer(0.f)
	, m_bShootState(false)
{
	ZeroMemory(&m_pWeapon, sizeof(void*) * (_uint)eWeaponType::WEAPON_END);
}

HRESULT CPlayer::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CPlayer::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fDeltaTime)
{	
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Key_Input(fDeltaTime);
	
	m_pWeapon[(_uint)m_pWeaponType]->Update_GameObject(fDeltaTime);

	return NO_EVENT;
}

_int CPlayer::LateUpdate_GameObject(const _float& fDeltaTime)
{

	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	//카메라 이동으로 인한 y값 보정 - 나중에 수정할것
	_vec3 vAPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	vAPos.y += m_fHeight;
	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vAPos);
	m_pTransformCom->Update_Component(fDeltaTime);
	_vec3 vPos = vAPos;
	vPos.y -= m_fHeight;
	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vPos);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI, this);

	m_pWeapon[(_uint)m_pWeaponType]->LateUpdate_GameObject(fDeltaTime);

	return NO_EVENT;
}

HRESULT CPlayer::Render_GameObject(void)
{	

	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (m_pWeaponType != eWeaponType::WEAPON_CROWBAR && m_pWeaponType != eWeaponType::WEAPON_PHYCANNON && m_pWeaponType != eWeaponType::WEAPON_END)
	{
		return Print_TestUI();
	}


	return S_OK;
}

void CPlayer::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CPlayer::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CPlayer::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CPlayer::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}
//휠 조작용
void CPlayer::MouseProc(UINT message, WPARAM wParam)
{
	if (message == WM_MOUSEWHEEL)
	{
		g_zDelta = (SHORT)HIWORD(wParam) < 0 ? 1.f : -1.f;
	}	
}

void CPlayer::Set_Animation(int iNumber)
{

}

Engine::CAnimationController* CPlayer::Get_AniController()
{
	return nullptr;
}

int CPlayer::Get_VertexNumber()
{
	return 0;
}

HRESULT CPlayer::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//아래는 위치 옮겨둘것

	m_pWeapon[(_uint)eWeaponType::WEAPON_CROWBAR] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponCrowbar"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_PISTOL] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponPistol"));
	
	m_pWeapon[(_uint)eWeaponType::WEAPON_SMG] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponSMG"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_RIFLE] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponRifle"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_SHOTGUN] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponShotgun"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_PHYCANNON] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponPhysCannon"));		
	
	m_pWeapon[(_uint)eWeaponType::WEAPON_RPG] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponRPG"));

	return S_OK;
}

HRESULT CPlayer::Print_TestUI()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	_tchar m_szAmmo[256];

	wsprintf(m_szAmmo, L"%d / %d", m_pWeapon[(_uint)m_pWeaponType]->Get_MagAmmo(), m_pWeapon[(_uint)(_uint)m_pWeaponType]->Get_RemainAmmo());
	pManagement->Render_Font(L"Font_BASE", m_szAmmo, &_vec2((WINCX >> 1) + (WINCX >> 2), WINCY - 20), D3DXCOLOR(1.0f, 1.0f, 1.f, 1.0f));

	return S_OK;
}

void CPlayer::Key_Input(const _float& fDeltaTime)
{
	auto* pManagement = Engine::CManagement::Get_Instance();
	NULL_CHECK(pManagement);

	_vec3 vLook, vPos, vRight;
	_mat matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);
	memcpy(&vLook, &matView.m[2][0], sizeof(_vec3));
	memcpy(&vRight, &matView.m[0][0], sizeof(_vec3));	

	if (pManagement->Key_Pressing('W'))
	{
		m_pTransformCom->Move_Pos(&vLook, 10.f, fDeltaTime);
	}
	if (pManagement->Key_Pressing('S'))
	{
		m_pTransformCom->Move_Pos(&vLook, 10.f, -fDeltaTime);
	}
	if (pManagement->Key_Pressing('A'))
	{
		m_pTransformCom->Move_Pos(&vRight, 10.f, -fDeltaTime);
	}
	if (pManagement->Key_Pressing('D'))
	{
		m_pTransformCom->Move_Pos(&vRight, 10.f, fDeltaTime);
	}

	//시선에 따른 Y축 이동 보정
	_vec3 vApPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	vApPos.y = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS).y;
	m_pTransformCom->Set_Pos(vApPos);

	//이후 점프 처리
	if (pManagement->Key_Down(VK_SPACE))
	{
		//점프
	}

	
	//사격
	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		m_pWeapon[(_uint)m_pWeaponType]->Shoot_Weapon();
		m_bShootState = true;

	}	
	if (pManagement->Key_Down(VK_RBUTTON))
	{
		m_pWeapon[(_uint)m_pWeaponType]->AltShoot_Weapon();
		m_bShootState = true;
	}
	if (!pManagement->Key_Pressing(VK_LBUTTON) && m_pWeapon[(_uint)m_pWeaponType]->Is_End_Animation())
	{
		m_pWeapon[(_uint)m_pWeaponType]->Release_Weapon();

		m_fWeaponTimer = 0.f;
		m_bShootState = false;
	}
	if (pManagement->Key_Down('R'))
	{
		m_bWeaponState = m_pWeapon[(_uint)m_pWeaponType]->Reload_Weapon();
	}



	if (g_zDelta > 0.1f)
	{
		m_pWeaponType = (eWeaponType)((_uint)m_pWeaponType+1);
		
		if (m_pWeaponType == eWeaponType::WEAPON_END)		//최종적으론 END
		{
			m_pWeaponType = eWeaponType::WEAPON_CROWBAR;
		}
	}
	else if (g_zDelta < -0.1f)
	{
		if (m_pWeaponType == eWeaponType::WEAPON_CROWBAR)
		{
			m_pWeaponType = eWeaponType::WEAPON_RPG;		//최종적으로는 마지막무기
		}
		else
		{
			m_pWeaponType = (eWeaponType)((_uint)m_pWeaponType - 1);
		}
	}
	g_zDelta = 0.f;

	if (m_bShootState == true)
	{
		m_fWeaponTimer += fDeltaTime;
	}

}

CPlayer* CPlayer::Create(_Device pDevice)
{
	CPlayer* pInstance = new CPlayer(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pClone = new CPlayer(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CPlayer");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CPlayer::Free()
{
	if (m_bIsPrototype == false)
	{
		m_pWeapon[(_uint)eWeaponType::WEAPON_CROWBAR]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_PISTOL]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_SMG]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_RIFLE]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_SHOTGUN]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_PHYCANNON]->Release();
		m_pWeapon[(_uint)eWeaponType::WEAPON_RPG]->Release();
	}


	Safe_Release(m_pSupportCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();


}
