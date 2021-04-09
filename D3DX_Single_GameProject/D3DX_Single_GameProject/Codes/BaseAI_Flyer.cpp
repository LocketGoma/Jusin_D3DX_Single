#include "framework.h"
#include "BaseAI_Flyer.h"

#include "Transform.h"

CBaseAI_Flyer::CBaseAI_Flyer(_Device pDevice)
	: CBaseAI(pDevice)
{
	m_fDodgeTime = 0.f;
	m_fInvinTime = 2.f;
	m_fDodgeCoolTime = 60.f;
	m_fDodgeCountTime = 0.f;

	m_bDodgeLock = false;

	m_iHPState = INT_MAX;
}

CBaseAI_Flyer::CBaseAI_Flyer(const CBaseAI_Flyer& other)
	: CBaseAI(other)
{
	m_bDodgeLock = false;

	m_iHPState = INT_MAX;
}

HRESULT CBaseAI_Flyer::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBaseAI_Flyer::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBaseAI_Flyer::Update_GameObject(const _float& fDeltaTime)
{
	//주그면 콰ㅇ
	if (m_pControlUnit->Hit_Attack(0))
	{
		m_pControlUnit->Set_Dead();
		m_bDead = true;
	}

	_vec3 vUnitPos = m_pControlUnit->Get_Position();
	_vec3 vTargetPos = m_pTargetUnit->Get_Position();

	_vec3 vRange = (vTargetPos - vUnitPos);
	m_fRangeToTarget = D3DXVec3Length(&vRange);

	_float fPosY = vUnitPos.y - vTargetPos.y;

	if (fabs(fPosY) < 2.45f)
	{
		m_eHeightState = eAlignHeight::Horizontal;
	}
	else if (fPosY > 0.f)
	{
		m_eHeightState = eAlignHeight::toDown;
	}
	else
	{
		m_eHeightState = eAlignHeight::toUp;
	}
	if (m_bAppear == false)
	{
		Do_Appear(fDeltaTime);
	}
	Do_Idle(fDeltaTime);
	if (m_bSpawn == false && m_bAppear == true)
	{
		Do_Movement(fDeltaTime);
		Do_Attack(fDeltaTime);
	}

	return NO_EVENT;

}

_int CBaseAI_Flyer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}

HRESULT CBaseAI_Flyer::Render_GameObject(void)
{
	return S_OK;
}
//필요시 사용
HRESULT CBaseAI_Flyer::Do_Spawn(const _float& fDeltaTime)
{
	m_bReady = true;

	return S_OK;
}

HRESULT CBaseAI_Flyer::Do_Appear(const _float& fDeltaTime)
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
	}


	return S_OK;
}

HRESULT CBaseAI_Flyer::Do_Idle(const _float& fDeltaTime)
{
	//수평 회전
	if (m_bAppear == true)
	{
		_vec3 vTargetPos;
		_vec3 vTargetLook;
		_vec3 vControlLook;
		_vec3 vCross;

		_mat matWorld;

		vTargetPos = m_pTargetUnit->Get_Position();
		vTargetLook = vTargetPos - m_pControlUnit->Get_Transform()->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
		vControlLook = m_pControlUnit->Get_Transform()->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
		D3DXVec3Normalize(&vTargetLook, &vTargetLook);
		D3DXVec3Normalize(&vControlLook, &vControlLook);
		D3DXVec3Cross(&vCross, &vControlLook, &vTargetLook);

		_float _fRotation = D3DXVec3Dot(&vTargetLook, &vControlLook);
		_float fDebug = D3DXToDegree(acos(_fRotation));
		_float fDebugB = D3DXVec3Length(&vCross);

		if (D3DXVec3Length(&vCross) > 0.01f)
		{
			if (vCross.y > 0)
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::RIGHT);
			else
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::LEFT);
		}

		//????
		if (m_eHeightState == eAlignHeight::toUp)
		{
			m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::DOWN);
		}
		else if (m_eHeightState == eAlignHeight::toDown)
		{
			m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::UP);
		}
		else if (m_eHeightState == eAlignHeight::Horizontal) 
		{
			m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::IDLE);
		}
	}
	return S_OK;
}

HRESULT CBaseAI_Flyer::Do_Movement(const _float& fDeltaTime)
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

HRESULT CBaseAI_Flyer::Do_Attack(const _float& fDeltaTime)
{
	//필요시 작성
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

CBaseAI_Flyer* CBaseAI_Flyer::Create(_Device pDevice)
{
	CBaseAI_Flyer* pInstance = new CBaseAI_Flyer(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBaseAI_Flyer::Clone(void* pArg)
{
	CBaseAI_Flyer* pClone = new CBaseAI_Flyer(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBaseAI_Flyer");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CBaseAI_Flyer::Free()
{
	CGameObject::Free();
}
