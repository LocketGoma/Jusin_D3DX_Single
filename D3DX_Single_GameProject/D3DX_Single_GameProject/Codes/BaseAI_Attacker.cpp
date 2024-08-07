#include "framework.h"
#include "BaseAI_Attacker.h"

#include "Transform.h"

CBaseAI_Attacker::CBaseAI_Attacker(_Device pDevice)
	: CBaseAI(pDevice)
{
	m_fDodgeTime = 0.f;
	m_fInvinTime = 2.f;
	m_fDodgeCoolTime = 7.5f;
	m_fDodgeCountTime = 0.f;

	m_bDodgeLock = false;

	m_iHPState = INT_MAX;
}

CBaseAI_Attacker::CBaseAI_Attacker(const CBaseAI_Attacker& other)
	: CBaseAI(other)

{
	m_bDodgeLock = false;

	m_iHPState = INT_MAX;
}

HRESULT CBaseAI_Attacker::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBaseAI_Attacker::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBaseAI_Attacker::Update_GameObject(const _float& fDeltaTime)
{
	if (m_pControlUnit->Hit_Attack(0))
	{
		m_eState = eAIStatus::DEAD;		
		m_pControlUnit->Do_Dead(fDeltaTime);
	}
	if (m_pControlUnit->Get_Clear_Dead_State())
	{
		m_pControlUnit->Set_Dead();
		m_bDead = true;
	}
	if (m_eState == eAIStatus::DEAD)
	{
		return OBJ_DEAD;
	}


	_vec3 vRange = (m_pTargetUnit->Get_Position()) - (m_pControlUnit->Get_Position());
	m_fRangeToTarget = D3DXVec3Length(&vRange);

	if (m_bReady == false)
	{
		Do_Spawn(fDeltaTime);
		return READY_EVENT;
	}

	if (m_bAppear == false)
	{
		Do_Appear(fDeltaTime);
	}
	else
	{
		Do_Idle(fDeltaTime);
	}
	if (m_bSpawn == false && m_bAppear == true && m_bDodge == false)
	{
		Do_Movement(fDeltaTime);
		Do_Attack(fDeltaTime);
	}
	if (m_bDodge == true)
	{
		m_bAttack = false;
		m_fDodgeTime += fDeltaTime;
	}

	return NO_EVENT;
}

_int CBaseAI_Attacker::LateUpdate_GameObject(const _float& fDeltaTime)
{
	//if (m_bAttack == true)
	//{
	//	__debugbreak();
	//}


	return NO_EVENT;
}

HRESULT CBaseAI_Attacker::Render_GameObject(void)
{
	return S_OK;
}

//필요시 사용
HRESULT CBaseAI_Attacker::Do_Spawn(const _float& fDeltaTime)
{
	if ((PLAYER_BASE_HITBOX + m_pControlUnit->Get_RecogRange()) >= m_fRangeToTarget)
	{
		m_bReady = true;
	}

	return S_OK;
}

//등장씬
HRESULT CBaseAI_Attacker::Do_Appear(const _float& fDeltaTime)
{

	if ((PLAYER_BASE_HITBOX + m_pControlUnit->Get_RecogRange()) >= m_fRangeToTarget && m_bSpawn == false)
	{
		m_pControlUnit->Do_Spawn(fDeltaTime);
		m_bSpawn = true;
	}

	if (m_bSpawn == true && m_pControlUnit->End_Animation_State_Force() == true)
	{
		m_bSpawn = false;
		m_bAppear = true;
		m_pControlUnit->Do_Idle(fDeltaTime);
	}


	return S_OK;
}
//기본 대기상태
HRESULT CBaseAI_Attacker::Do_Idle(const _float& fDeltaTime)
{
	if (m_bAppear == true)
	{
		_vec3 vTargetPos;
		_vec3 vTargetLook;
		_vec3 vControlPos;
		_vec3 vControlLook;
		_vec3 vCross;

		_mat matWorld;

		vTargetPos = m_pTargetUnit->Get_Position();
		vTargetLook = vTargetPos - m_pControlUnit->Get_Transform()->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
		vControlPos = m_pControlUnit->Get_Position();
		vControlLook = m_pControlUnit->Get_Transform()->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
		D3DXVec3Normalize(&vTargetLook, &vTargetLook);
		D3DXVec3Normalize(&vControlLook, &vControlLook);
		D3DXVec3Cross(&vCross, &vControlLook, &vTargetLook);

		if (D3DXVec3Length(&vCross) > 1/D3DXVec3Length(&(vTargetPos- vControlPos)) * 6.f)
		{
			//이게 최선인 것인가?
			if (vCross.y > 0)
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::RIGHT);
			else
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::LEFT);
		}
	}
	if (Check_HP_Change() && m_bDodgeLock == false)
	{
		m_pControlUnit->Go_Side(fDeltaTime, (eAlign)(rand() % 2));
		m_bDodge = true;
		m_bDodgeLock = true;
	}
	else if (m_fDodgeTime >= m_fInvinTime || m_pControlUnit->Do_Dodge(fDeltaTime) == true)
	{
		m_bDodge = false;
		m_fDodgeTime = 0.f;
	}

	if (m_bDodgeLock == true)
	{
		m_fDodgeCountTime += fDeltaTime;
	}
	if (m_fDodgeCountTime >= m_fDodgeCoolTime)
	{
		m_bDodgeLock = false;
		m_fDodgeCountTime = 0.f;
	}

	return S_OK;
}
//기본 움직임
HRESULT CBaseAI_Attacker::Do_Movement(const _float& fDeltaTime)
{
	if (m_bAttack == true)
	{
		m_bTrack = false;
		return S_OK;
	}

	if (m_bDodge == false && m_bTrack == false && (PLAYER_BASE_HITBOX + m_pControlUnit->Get_MoveRange()) >= m_fRangeToTarget)
	{
		m_bTrack = true;
	}
	else
	{
		m_bTrack = false;
	}

	if (m_bTrack == true)
	{
		m_pControlUnit->Go_Stright(fDeltaTime);
	}


	return S_OK;
}

HRESULT CBaseAI_Attacker::Do_Attack(const _float& fDeltaTime)
{

	if (m_fRangeToTarget <= PLAYER_BASE_HITBOX + m_pControlUnit->Get_AttackRange())
	{
		m_bAttack = true;
		m_bTrack = false;
	}
	else if (m_bTrack == false)
	{
		m_bAttack = false;
	}
	else if (m_bTrack == false && m_bAttack == false)
	{
		m_pControlUnit->Do_Idle(fDeltaTime);
	}

	if (m_bAttack == true)
	{
		if (m_fRangeToTarget <= PLAYER_BASE_HITBOX * 3)
		{
			m_eBasePatton = eBasePatton::PattonA;
		}
		else if ((eBasePatton)m_pControlUnit->Get_Patton() != eBasePatton::PattonB && (eBasePatton)m_pControlUnit->Get_Patton() != eBasePatton::PattonC)
		{
			m_eBasePatton = (eBasePatton)(rand() % 2 + 3);
		}
			m_pControlUnit->Do_Attack(fDeltaTime, (_uint)m_eBasePatton);
	}

	return S_OK;
}

CBaseAI_Attacker* CBaseAI_Attacker::Create(_Device pDevice)
{
	CBaseAI_Attacker* pInstance = new CBaseAI_Attacker(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBaseAI_Attacker::Clone(void* pArg)
{
	CBaseAI_Attacker* pClone = new CBaseAI_Attacker(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBaseAI_Attacker");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CBaseAI_Attacker::Free()
{
	CGameObject::Free();
}
