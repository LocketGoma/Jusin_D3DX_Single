#include "framework.h"
#include "BaseAI.h"

#include "Transform.h"

CBaseAI::CBaseAI(_Device pDevice)
	: CGameObject(pDevice)
	, m_pControlUnit(nullptr)
	, m_pTargetUnit(nullptr)
	, m_bSpawn(false)
	, m_bAppear(false)
	, m_bTrack(false)
	, m_bAttack(false)
{
}

CBaseAI::CBaseAI(const CBaseAI& other)
	: CGameObject(other)
	, m_pControlUnit(other.m_pControlUnit)
	, m_pTargetUnit(other.m_pTargetUnit)
	, m_bSpawn(false)
	, m_bAppear(false)
	, m_bTrack(false)
	, m_bAttack(false)
{
}

HRESULT CBaseAI::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBaseAI::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBaseAI::Update_GameObject(const _float& fDeltaTime)
{
	if (m_pControlUnit->Hit_Attack(0))
	{
		m_pControlUnit->Set_Dead();
		m_bDead = true;
	}

	_vec3 vRange = (m_pTargetUnit->Get_Position()) - (m_pControlUnit->Get_Position());
	m_fRangeToTarget = D3DXVec3Length(&vRange);

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
	return _int();
}

_int CBaseAI::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CBaseAI::Render_GameObject(void)
{
	return S_OK;
}


void CBaseAI::Set_Position(_vec3 vPos)
{
}
void CBaseAI::Set_Size(_vec3 vSize)
{
}
_vec3 CBaseAI::Get_Position()
{
	return _vec3();
}
_vec3 CBaseAI::Get_Size()
{
	return _vec3();
}

HRESULT CBaseAI::Set_ControlUnit(CDynamicObject* pUnit)
{
	NULL_CHECK_RETURN(pUnit, E_FAIL);

	m_pControlUnit = pUnit;
}

HRESULT CBaseAI::Set_Target(Engine::CGameObject* pTarget)
{
	NULL_CHECK_RETURN(pTarget, E_FAIL);

	m_pTargetUnit = pTarget;
}

HRESULT CBaseAI::Do_Appear(const _float& fDeltaTime)
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

HRESULT CBaseAI::Do_Idle(const _float& fDeltaTime)
{
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
			//이게 최선인 것인가?
			if (vCross.y > 0)
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::RIGHT);
			else
				m_pControlUnit->Do_Rotate(fDeltaTime, eAlign::LEFT);
		}
	}
	
	return S_OK;
}

HRESULT CBaseAI::Do_Movement(const _float& fDeltaTime)
{
	if (m_bAttack == true)
	{
		m_bTrack = false;
		return S_OK;
	}

	if (m_bTrack == false && (PLAYER_BASE_HITBOX + m_pControlUnit->Get_MoveRange()) >= m_fRangeToTarget)
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

HRESULT CBaseAI::Do_Attack(const _float& fDeltaTime)
{

	if (m_fRangeToTarget <= PLAYER_BASE_HITBOX + m_pControlUnit->Get_AttackRange())
	{
		m_bAttack = true;
	}
	else
	{
		m_pControlUnit->Do_Idle(fDeltaTime);
		m_bAttack = false;
	}

	if (m_bAttack == true)
	{
		m_pControlUnit->Do_Attack(fDeltaTime);
	}

	return S_OK;
}

CBaseAI* CBaseAI::Create(_Device pDevice)
{
	CBaseAI* pInstance = new CBaseAI(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CBaseAI::Clone(void* pArg)
{
	CBaseAI* pClone = new CBaseAI(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CBaseAI");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CBaseAI::Free()
{
	CGameObject::Free();
}

