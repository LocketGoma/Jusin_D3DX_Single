#include "framework.h"
#include "Player.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "ControlSupport.h"

#include "CameraComponent.h"

//����

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

	//ī�޶� �̵����� ���� y�� ���� - ���߿� �����Ұ�
	_float fY = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS).y;
	m_pTransformCom->Update_Component(fDeltaTime);
	_vec3 vPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
	//vPos.y = fY;
	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vPos);

	
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

	m_pTransformCom->LateUpdate_Component(0.f);

	m_pWeapon[(_uint)m_pWeaponType]->LateUpdate_GameObject(fDeltaTime);

	return NO_EVENT;
}

HRESULT CPlayer::Render_GameObject(void)
{

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

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

	//�Ʒ��� ��ġ �ŰܵѰ�

	m_pWeapon[(_uint)eWeaponType::WEAPON_CROWBAR] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponCrowbar"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_PISTOL] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponPistol"));
	
	m_pWeapon[(_uint)eWeaponType::WEAPON_SMG] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponSMG"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_RIFLE] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponRifle"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_SHOTGUN] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponShotgun"));

	m_pWeapon[(_uint)eWeaponType::WEAPON_PHYCANNON] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponPhysCannon"));		
	
	m_pWeapon[(_uint)eWeaponType::WEAPON_RPG] = dynamic_cast<CPlayerWeapon*>(pManagement->Clone_GameObject(L"WeaponRPG"));

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
	//���
	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		m_pWeapon[(_uint)m_pWeaponType]->Shoot_Weapon();
		m_bShootState = true;

	}	
	if (pManagement->Key_Pressing(VK_RBUTTON))
	{
		m_pWeapon[(_uint)m_pWeaponType]->AltShoot_Weapon();
		m_bShootState = true;
	}
	if (pManagement->Key_Up(VK_LBUTTON) || m_fWeaponTimer >= m_fMaxWeaponTimer)
	{
		m_pWeapon[(_uint)m_pWeaponType]->Release_Weapon();

		m_fWeaponTimer = 0.f;
		m_bShootState = false;
	}

	if (g_zDelta > 0.1f)
	{
		m_pWeaponType = (eWeaponType)((_uint)m_pWeaponType+1);
		
		if (m_pWeaponType == eWeaponType::WEAPON_END)		//���������� END
		{
			m_pWeaponType = eWeaponType::WEAPON_CROWBAR;
		}
	}
	else if (g_zDelta < -0.1f)
	{
		if (m_pWeaponType == eWeaponType::WEAPON_CROWBAR)
		{
			m_pWeaponType = eWeaponType::WEAPON_RPG;		//���������δ� ����������
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
