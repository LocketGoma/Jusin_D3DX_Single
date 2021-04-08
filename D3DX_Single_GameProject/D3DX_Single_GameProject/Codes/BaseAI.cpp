#include "framework.h"
#include "BaseAI.h"

#include "Transform.h"

CBaseAI::CBaseAI(_Device pDevice)
	: CGameObject(pDevice)
	, m_pControlUnit(nullptr)
	, m_pTargetUnit(nullptr)
	, m_bReady(false)
	, m_bSpawn(false)
	, m_bAppear(false)
	, m_bTrack(false)
	, m_bAttack(false)
	, m_bDodge(false)
	, m_eBasePatton(eBasePatton::Idle)
	, m_fRangeToTarget(0.f)
	, m_fDodgeTime(0.f)
	, m_fInvinTime(1.25f)
	, m_fDodgeCoolTime(0.f)
	, m_fDodgeCountTime(0.f)
{
	m_iHPState = INT_MAX;
}

CBaseAI::CBaseAI(const CBaseAI& other)
	: CGameObject(other)
	, m_pControlUnit(other.m_pControlUnit)
	, m_pTargetUnit(other.m_pTargetUnit)
	, m_bReady(false)
	, m_bSpawn(false)
	, m_bAppear(false)
	, m_bTrack(false)
	, m_bAttack(false)
	, m_bDodge(false)
	, m_eBasePatton(other.m_eBasePatton)
	, m_fRangeToTarget(other.m_fRangeToTarget)
	, m_fDodgeTime(other.m_fDodgeTime)
	, m_fInvinTime(other.m_fInvinTime)
	, m_fDodgeCoolTime(other.m_fDodgeCoolTime)
	, m_fDodgeCountTime(other.m_fDodgeCountTime)
{
	m_iHPState = INT_MAX;
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

	return S_OK;
}

HRESULT CBaseAI::Set_Target(Engine::CGameObject* pTarget)
{
	NULL_CHECK_RETURN(pTarget, E_FAIL);

	m_pTargetUnit = pTarget;

	return S_OK;
}

_bool CBaseAI::Check_HP_Change()
{
	if (m_iHPState != m_pControlUnit->Get_NowHP())
	{
		if (m_iHPState != INT_MAX)
		{	
			m_iHPState = m_pControlUnit->Get_NowHP();
			return true;
		}
		else
		{
			m_iHPState = m_pControlUnit->Get_NowHP();
		}
	}

	return false;
}

void CBaseAI::Free()
{
	CGameObject::Free();
}

