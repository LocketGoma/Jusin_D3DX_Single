#include "framework.h"
#include "BossAI_Strider.h"

#include "Transform.h"

#include "BossStrider.h"

CBossAI_Strider::CBossAI_Strider(_Device pDevice)
	: CBaseAI(pDevice)
{
	m_fDodgeTime = 0.f;
	m_fInvinTime = 2.5f;
	m_fDodgeCoolTime = 2.5f;
	m_fDodgeCountTime = 0.f;

	m_bDodgeLock = false;
	m_iHPState = INT_MAX;
}

CBossAI_Strider::CBossAI_Strider(const CBossAI_Strider& other)
	:CBaseAI(other)
{
	m_bDodgeLock = false;
	m_iHPState = INT_MAX;
}

HRESULT CBossAI_Strider::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBossAI_Strider::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBossAI_Strider::Update_GameObject(const _float& fDeltaTime)
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

	_vec3 vControlUnitPos = m_pControlUnit->Get_Position();
	_vec3 vTargetPos = m_pTargetUnit->Get_Position();
	vControlUnitPos.y = vTargetPos.y;

	_vec3 vRange = (vTargetPos) - (vControlUnitPos);
	m_fRangeToTarget = D3DXVec3Length(&vRange);

	dynamic_cast<CBossStrider*>(m_pControlUnit)->Set_TargetPosition(vTargetPos);

	if (m_bAppear == false)
	{
		Do_Appear(fDeltaTime);
	}
	Do_Idle(fDeltaTime);
	if (m_bSpawn == false && m_bAppear == true)
	{
		//Do_Movement(fDeltaTime);
		Do_Attack(fDeltaTime);
	}

	return NO_EVENT;
}

_int CBossAI_Strider::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}

HRESULT CBossAI_Strider::Render_GameObject(void)
{
	return S_OK;
}

HRESULT CBossAI_Strider::Do_Spawn(const _float& fDeltaTime)
{
	return E_NOTIMPL;
}

HRESULT CBossAI_Strider::Do_Appear(const _float& fDeltaTime)
{
	if ((PLAYER_BASE_HITBOX + m_pControlUnit->Get_RecogRange()) >= m_fRangeToTarget)
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

HRESULT CBossAI_Strider::Do_Idle(const _float& fDeltaTime)
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

		if (D3DXVec3Length(&vCross) > 1 / D3DXVec3Length(&(vTargetPos - vControlPos)) * 25.f)
		{
			//이게 최선인 것인가?
			if (vCross.y > 0)
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::RIGHT);
			else
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::LEFT);
		}
	}

	return S_OK;
}

HRESULT CBossAI_Strider::Do_Movement(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBossAI_Strider::Do_Attack(const _float& fDeltaTime)
{
	if (m_fRangeToTarget <= PLAYER_BASE_HITBOX + m_pControlUnit->Get_AttackRange())
	{
		m_bAttack = true;
		m_bTrack = false;
	}
	else
	{
		m_bAttack = false;
		m_pControlUnit->Do_Idle(fDeltaTime);
	}

	if (m_bAttack == true)
	{
		if (m_fRangeToTarget <= PLAYER_BASE_HITBOX * 4.f)
		{
			m_eBasePatton = eBasePatton::PattonA;
		}
		else if((eBasePatton)m_pControlUnit->Get_Patton() != eBasePatton::PattonB && (eBasePatton)m_pControlUnit->Get_Patton() != eBasePatton::PattonC && (eBasePatton)m_pControlUnit->Get_Patton() != eBasePatton::PattonD)
		{
			//m_eBasePatton = eBasePatton::PattonB;
			m_eBasePatton = (eBasePatton)(rand() % 3 + 3);
		}
		m_pControlUnit->Do_Attack(fDeltaTime, (_uint)m_eBasePatton);
	}


	return S_OK;
}

CBossAI_Strider* CBossAI_Strider::Create(_Device pDevice)
{
	CBossAI_Strider* pInstance = new CBossAI_Strider(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBossAI_Strider::Clone(void* pArg)
{
	CBossAI_Strider* pClone = new CBossAI_Strider(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBossAI_Strider");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CBossAI_Strider::Free()
{
	CBaseAI::Free();
}
