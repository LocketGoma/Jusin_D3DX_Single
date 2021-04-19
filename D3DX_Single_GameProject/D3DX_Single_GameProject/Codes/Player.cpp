#include "framework.h"
#include "Player.h"

#include "DynamicMesh.h"
#include "Transform.h"
#include "ControlSupport.h"

#include "CameraComponent.h"

#include "SphereCollider.h"

//오브젝트 상호작용
#include "BaseObject.h"

//무기
#include "PlayerWeapon.h"
#include "WeaponCrowBar.h"
#include "WeaponPhysCannon.h"

#include "StatusUI.h"
#include "CrossHairUI.h"
#include "WeaponUI.h"

float CPlayer::g_zDelta = 0.f;

CPlayer::CPlayer(_Device pDevice)
	: Engine::CGameObject(pDevice)
	, m_pWeaponType(eWeaponType::WEAPON_CROWBAR)
	, m_pPrevWeaponType(eWeaponType::WEAPON_CROWBAR)
	, m_fWeaponTimer(0.f)
	, m_bShootState(false)
	, m_fInteractionRange(15.f)
	, m_iFullHP(100)
	, m_iShieldFullHP(50)
	, m_fHitboxSize(5.0f)
	, eType(Engine::COLIDETYPE::COL_FALSE)
	, m_fWalkSpeed(10.f)
	, m_fRunSpeed(20.f)
	, m_fJumpPower(7.55f)
	, m_fNowJumpPos(0.f)
	, m_fGravition(9.8f)
	, m_bJump(false)
	, m_bJumpStart(false)
	, m_bSprint(false)
	, m_bLowHP(false)

	
{
	ZeroMemory(&m_pWeapon, sizeof(void*) * (_uint)eWeaponType::WEAPON_END);

	m_iHP = m_iFullHP;
	m_ibHP = m_iHP;

	m_iShieldHP = m_iShieldFullHP;
	m_ibShieldHP = m_iShieldHP;
}

CPlayer::CPlayer(const CPlayer& other)
	: Engine::CGameObject(other)
	, m_pWeaponType(other.m_pWeaponType)
	, m_pPrevWeaponType(other.m_pPrevWeaponType)
	, m_fWeaponTimer(0.f)
	, m_bShootState(false)
	, m_fInteractionRange(other.m_fInteractionRange)
	, m_iFullHP(other.m_iFullHP)
	, m_iShieldFullHP(other.m_iShieldFullHP)
	, m_fHitboxSize(other.m_fHitboxSize)
	, eType(Engine::COLIDETYPE::COL_FALSE)
	, m_fWalkSpeed(other.m_fWalkSpeed)
	, m_fRunSpeed(other.m_fRunSpeed)
	, m_fJumpPower(other.m_fJumpPower)
	, m_fNowJumpPos(other.m_fNowJumpPos)
	, m_fGravition(other.m_fGravition)
	, m_bJump(false)
	, m_bJumpStart(false)
	, m_bSprint(false)
	, m_bLowHP(false)
{
	ZeroMemory(&m_pWeapon, sizeof(void*) * (_uint)eWeaponType::WEAPON_END);

	m_iHP = m_iFullHP;
	m_ibHP = m_iHP;

	m_iShieldHP = m_iShieldFullHP;
	m_ibShieldHP = m_iShieldHP;

	m_fNowMoveSpeed = m_fWalkSpeed;
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
	Engine::CGameObject::Update_GameObject(fDeltaTime* DEBUG_TIMESPEED);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Key_Input(fDeltaTime* DEBUG_TIMESPEED);
	
	m_pWeapon[(_uint)m_pWeaponType]->Update_GameObject(fDeltaTime* DEBUG_TIMESPEED);

	return NO_EVENT;
}

_int CPlayer::LateUpdate_GameObject(const _float& fDeltaTime)
{

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Jump_Action(fDeltaTime);

	//카메라 이동으로 인한 y값 보정 - 나중에 수정할것
	_vec3 vAPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	vAPos.y += m_fHeight;
	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vAPos);
	m_pTransformCom->Update_Component(fDeltaTime);
	_vec3 vPos = vAPos;
	vPos.y -= m_fHeight;
	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_POS, &vPos);

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI, this);

	m_pWeapon[(_uint)m_pWeaponType]->LateUpdate_GameObject(fDeltaTime* DEBUG_TIMESPEED);

	if (m_pStatusUI != nullptr)
	{
		m_pStatusUI->Set_HP(m_iHP);
		m_pStatusUI->Set_SuitHP(m_iShieldHP);

		if (m_pWeaponType != eWeaponType::WEAPON_CROWBAR && m_pWeaponType != eWeaponType::WEAPON_PHYCANNON)
		{
			m_pStatusUI->Set_MagAmmo(m_pWeapon[(_uint)m_pWeaponType]->Get_MagAmmo());
			m_pStatusUI->Set_Ammo(m_pWeapon[(_uint)m_pWeaponType]->Get_RemainAmmo());
			m_pStatusUI->Set_AltAmmo(m_pWeapon[(_uint)m_pWeaponType]->Get_RemainAltAmmo());
			m_pStatusUI->Set_EffectVisualble(eStatusType::AMMO, true);
		}
		else
		{
			m_pStatusUI->Set_EffectVisualble(eStatusType::AMMO, false);
		}
		m_pStatusUI->LateUpdate_GameObject(fDeltaTime);
	}
	if (m_pCrossHairUI != nullptr)
	{
		m_pCrossHairUI->Set_HP(m_iHP);
		
		if (m_pWeaponType != eWeaponType::WEAPON_CROWBAR && m_pWeaponType != eWeaponType::WEAPON_PHYCANNON)
		{
			m_pCrossHairUI->Set_MagAmmo(m_pWeapon[(_uint)m_pWeaponType]->Get_MagAmmo());
			m_pCrossHairUI->Set_MagFullAmmo(m_pWeapon[(_uint)m_pWeaponType]->Get_MaxMagAmmo());
		}
		else
		{
			m_pCrossHairUI->Set_MagAmmo(-1);
			m_pCrossHairUI->Set_MagFullAmmo(-1);
		}
		m_pCrossHairUI->LateUpdate_GameObject(fDeltaTime);
	}
	if (m_pWeaponUI != nullptr)
	{
		if (m_pPrevWeaponType != m_pWeaponType)
		{
			m_pWeaponUI->Select_Weapon((_uint)m_pWeaponType);
		}
		m_pWeaponUI->LateUpdate_GameObject(fDeltaTime);
	}

	m_pPrevWeaponType = m_pWeaponType;


	// 기타 효과음
	if (m_iShieldHP == 45 && m_bLowShield == false)
	{
		m_bLowShield = true;
		pManagement->Stop_Sound(Engine::SOUND_CHANNELID::PLAYERSUIT);
		pManagement->Play_Sound(L"hev_45p.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
	}
	else if (m_iShieldFullHP > 45)
	{
		m_bLowShield = false;
	}

	if (m_bLowHP == true && !pManagement->Check_PlaySound(Engine::SOUND_CHANNELID::PLAYERSUIT))
	{
		pManagement->Stop_Sound(Engine::SOUND_CHANNELID::PLAYERSUIT);
		switch(rand() % 4)
		{
			case 0:
				pManagement->Play_Sound(L"hev_hlth2.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			case 1:
				pManagement->Play_Sound(L"hev_hlth3.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			case 2:
				pManagement->Play_Sound(L"hev_hlth5.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			case 3:
				pManagement->Play_Sound(L"hev_hlth6.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
		}
		m_bLowHP = false;
	}


	return NO_EVENT;
}

HRESULT CPlayer::Render_GameObject(void)
{	

	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	return Print_TestUI();

	//return S_OK;
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
_bool CPlayer::Get_Pick_Object(CBaseObject* pObject, _float fRange)
{
	//타겟 오브젝트가 없을때 & 너무 멀때
	if (pObject == nullptr)
	{
		if (m_pWeaponType == eWeaponType::WEAPON_PHYCANNON)
		{
			dynamic_cast<CWeaponPhysCannon*>(m_pWeapon[(_uint)m_pWeaponType])->Set_TargetObject(nullptr);
		}

		return false;
	}

	//중력건은 상호작용 거리 2배
	if (m_pWeaponType == eWeaponType::WEAPON_PHYCANNON && fRange <= m_fInteractionRange * 3)
	{
		dynamic_cast<CWeaponPhysCannon*>(m_pWeapon[(_uint)m_pWeaponType])->Set_TargetObject(pObject);
	}
	else if (fRange <= m_fInteractionRange)
	{
		;
	}
	return true;
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

_uint CPlayer::Get_WeaponDamage()
{
	if (m_bShootState == true)
		return m_pWeapon[(_uint)m_pWeaponType]->Get_Damage_Primary();
	else
		return 0;
}

bool CPlayer::Hit_Attack(_uint iDamage)
{
	if (m_iShieldHP > 0)
	{
		m_iShieldHP -= iDamage / 2;

		if (m_iShieldHP < 0)
		{
			m_iHP += m_iShieldHP;
			m_iShieldHP = 0;
		}
	}
	else
	{
		m_iShieldHP = 0;
		m_iHP -= iDamage;
	}

	if (m_iHP <= 0)
	{
		m_iHP = 0;
		return true;
	}
	return false;
}

_bool CPlayer::Restore_HP(_uint iAmount)
{
	if (m_iHP >= m_iFullHP)
		return false;

	m_iHP += iAmount;

	if (m_iHP > m_iFullHP)
		m_iHP = m_iFullHP;

	return true;
}

_bool CPlayer::Restore_Shield(_uint iAmount)
{
	if (m_iShieldHP >= m_iShieldFullHP)
		return false;

	m_iShieldHP += iAmount;

	if (m_iShieldHP > m_iShieldFullHP)
		m_iShieldHP = m_iShieldFullHP;

	return true;
}

_uint CPlayer::Get_HP()
{
	return m_iHP;
}

_uint CPlayer::Get_ShieldHP()
{
	return m_iShieldHP;
}

_bool CPlayer::HurtState()
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return false;
	}

	if (m_ibHP > m_iHP || m_ibShieldHP > m_iShieldHP)
	{
		if (m_iShieldHP == 0)
		{
			switch (rand() % 3)
			{
			case 0:
				pManagement->Play_Sound(L"hev_dmg2.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			case 1:
				pManagement->Play_Sound(L"hev_dmg4.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			case 2:
				pManagement->Play_Sound(L"hev_dmg6.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
				break;
			}

			if (m_iHP < 45)
			{
				m_bLowHP = true;
			}
		}

		switch (rand() % 4)
		{
		case 0:
			pManagement->Play_Sound(L"pl_fallpain1.wav", Engine::SOUND_CHANNELID::PLAYER);
			break;
		case 1:
			pManagement->Play_Sound(L"pl_pain5.wav", Engine::SOUND_CHANNELID::PLAYER);
			break;
		case 2:
			pManagement->Play_Sound(L"pl_pain6.wav", Engine::SOUND_CHANNELID::PLAYER);
			break;
		case 3:
			pManagement->Play_Sound(L"pl_pain7.wav", Engine::SOUND_CHANNELID::PLAYER);
			break;
		}


		m_ibHP = m_iHP;
		m_ibShieldHP = m_iShieldHP;

		return true;
	}


	return false;
}

_bool CPlayer::Get_Recoil()
{
	return m_bShootState;
}

_float CPlayer::Get_RecoilPower()
{
	return m_pWeapon[(_uint)m_pWeaponType]->Get_Recoil_Power();
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);


	m_pStatusUI = dynamic_cast<CStatusUI*>(pManagement->Clone_GameObject(L"StatusUI"));
	
	m_pCrossHairUI = dynamic_cast<CCrossHairUI*>(pManagement->Clone_GameObject(L"CrossHairUI"));
	
	m_pWeaponUI = dynamic_cast<CWeaponUI*>(pManagement->Clone_GameObject(L"WeaponUI"));


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

	_tchar m_szPos[256];

	_vec3 vPos = Get_Position();
	_stprintf_s(m_szPos, L"x : %f \ny :%f \nz : %f", vPos.x,vPos.y,vPos.z);
	pManagement->Render_Font(L"Font_BASE", m_szPos, &_vec2(0,60), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));


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
 		m_pTransformCom->Move_Pos(&vLook, m_fNowMoveSpeed, fDeltaTime);
	}
	if (pManagement->Key_Pressing('S'))
	{
		m_pTransformCom->Move_Pos(&vLook, m_fNowMoveSpeed, -fDeltaTime);
	}
	if (pManagement->Key_Pressing('A'))
	{
		m_pTransformCom->Move_Pos(&vRight, m_fNowMoveSpeed, -fDeltaTime);
	}
	if (pManagement->Key_Pressing('D'))
	{
		m_pTransformCom->Move_Pos(&vRight, m_fNowMoveSpeed, fDeltaTime);
	}
	if (pManagement->Key_Pressing(VK_LSHIFT))
	{
		if (m_bSprint == false)
		{
			pManagement->Stop_Sound(Engine::SOUND_CHANNELID::PLAYERSUIT);
			pManagement->Play_Sound(L"suit_sprint.wav", Engine::SOUND_CHANNELID::PLAYERSUIT);
		
			m_bSprint = true;
		}

		m_fNowMoveSpeed = m_fRunSpeed;
	}
	else
	{
		m_bSprint = false;
		m_fNowMoveSpeed = m_fWalkSpeed;
	}	
	if (pManagement->Key_Pressing(VK_SPACE))
	{
		m_bJump = true;
	}

	//시선에 따른 Y축 이동 보정
	_vec3 vApPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	vApPos.y = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS).y;
	m_pTransformCom->Set_Pos(vApPos);



	
	//사격
	m_bShootState = false;
	if (pManagement->Key_Pressing(VK_LBUTTON))
	{
		m_bShootState = m_pWeapon[(_uint)m_pWeaponType]->Shoot_Weapon();		
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


	//무기 전환

	if (pManagement->Key_Down('1'))
	{
		m_pWeaponType = eWeaponType::WEAPON_CROWBAR;
	}
	if (pManagement->Key_Down('2'))
	{
		m_pWeaponType = eWeaponType::WEAPON_PISTOL;
	}
	if (pManagement->Key_Down('3'))
	{
		m_pWeaponType = eWeaponType::WEAPON_SMG;
	}
	if (pManagement->Key_Down('4'))
	{
		m_pWeaponType = eWeaponType::WEAPON_RIFLE;
	}
	if (pManagement->Key_Down('5'))
	{
		m_pWeaponType = eWeaponType::WEAPON_SHOTGUN;
	}
	if (pManagement->Key_Down('6'))
	{
		m_pWeaponType = eWeaponType::WEAPON_PHYCANNON;
	}
	if (pManagement->Key_Down('7'))
	{
		m_pWeaponType = eWeaponType::WEAPON_RPG;
	}


	if (g_zDelta > 0.1f)
	{
		m_pWeapon[(_uint)m_pWeaponType]->Change_Weapon();

		m_pWeaponType = (eWeaponType)((_uint)m_pWeaponType+1);
		
		if (m_pWeaponType == eWeaponType::WEAPON_END)		//최종적으론 END
		{
			m_pWeaponType = eWeaponType::WEAPON_CROWBAR;
		}
		m_bShootState = false;
	}
	else if (g_zDelta < -0.1f)
	{
		m_pWeapon[(_uint)m_pWeaponType]->Change_Weapon();

		if (m_pWeaponType == eWeaponType::WEAPON_CROWBAR)
		{
			m_pWeaponType = eWeaponType::WEAPON_RPG;		//최종적으로는 마지막무기
		}
		else
		{
			m_pWeaponType = (eWeaponType)((_uint)m_pWeaponType - 1);
		}
		m_bShootState = false;
	}
	g_zDelta = 0.f;

	if (m_bShootState == true)
	{
		m_fWeaponTimer += fDeltaTime;
	}
}

_bool CPlayer::Jump_Action(const _float& fDeltaTime)
{
	if (m_bJump == true && m_bJumpStart == false)
	{		
		m_bJumpStart = true;

		m_fStartPos = Get_Position().y;
	}
	if (m_bJump)
	{
		m_fJumpTime += fDeltaTime;

		m_fNowJumpPos = m_fJumpPower * m_fJumpTime - (m_fGravition * m_fJumpTime * m_fJumpTime) / 2;

		_vec3 vNowPos = _vec3(0.f, 0.f, 0.f);
		vNowPos.y = m_fNowJumpPos;
		m_pTransformCom->Add_Pos(vNowPos);
	}
	
	//_vec3 vPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);

	//if (m_fStartPos > vPos.y)
	//{
	//	vPos.y = m_fStartPos;
	//	m_pTransformCom->Set_Pos(vPos);


	//	m_fJumpTime = 0.f;
	//	m_bJump = false;
	//	m_bJumpStart = false;
	//	return false;
	//}


	return true;
}

_bool CPlayer::Check_Attack_Collide(const _vec3* pSourcePos, const _float fSourceRadius)
{
	_bool bReturn = m_pSupportCom->Collision_Sphere(&(this->Get_Position()), this->m_fHitboxSize, pSourcePos, fSourceRadius);

	bReturn == true ? eType = Engine::COLIDETYPE::COL_TRUE : eType = Engine::COLIDETYPE::COL_FALSE;

	return bReturn;
}

void CPlayer::Jump_Cancel()
{
	m_bJumpStart = false;
	m_bJump = false;

	m_fJumpTime = 0.f;
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

		m_pStatusUI->Release();
		m_pCrossHairUI->Release();
		m_pWeaponUI->Release();
	}


	Safe_Release(m_pSupportCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();


}
