#include "framework.h"
#include "BossAI_Hunter.h"

CBossAI_Hunter::CBossAI_Hunter(_Device pDevice)
	: CBaseAI(pDevice)
{
	m_fDodgeTime = 0.f;

	m_iHPState = INT_MAX;
}

CBossAI_Hunter::CBossAI_Hunter(const CBossAI_Hunter& other)
	: CBaseAI(other)
{
	m_fDodgeTime = 0.f;

	m_iHPState = INT_MAX;
}

HRESULT CBossAI_Hunter::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBossAI_Hunter::Update_GameObject(const _float& fDeltaTime)
{
	//»ç¸Á ÆÇÁ¤
	if (m_pControlUnit->Hit_Attack(0))
	{
		m_pControlUnit->Set_Dead();
		m_bDead = true;
	}
	_vec3 vRange = (m_pTargetUnit->Get_Position()) - (m_pControlUnit->Get_Position());
	m_fRangeToTarget = D3DXVec3Length(&vRange);


	return _int();
}

_int CBossAI_Hunter::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CBossAI_Hunter::Render_GameObject(void)
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Do_Spawn(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Do_Appear(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Do_Idle(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Do_Movement(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBossAI_Hunter::Do_Attack(const _float& fDeltaTime)
{
	return S_OK;
}

CBossAI_Hunter* CBossAI_Hunter::Create(_Device pDevice)
{
	return nullptr;
}

Engine::CGameObject* CBossAI_Hunter::Clone(void* pArg)
{
	return nullptr;
}

void CBossAI_Hunter::Free()
{
}
