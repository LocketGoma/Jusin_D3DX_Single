#include "framework.h"
#include "BaseAI_Shooter.h"

CBaseAI_Shooter::CBaseAI_Shooter(_Device pDevice)
	: CBaseAI(pDevice)
{
}

CBaseAI_Shooter::CBaseAI_Shooter(const CBaseAI_Shooter& other)
	: CBaseAI(other)
{
}

HRESULT CBaseAI_Shooter::Ready_GameObject()
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CBaseAI_Shooter::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CBaseAI_Shooter::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CBaseAI_Shooter::Render_GameObject(void)
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Do_Spawn(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Do_Appear(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Do_Idle(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Do_Movement(const _float& fDeltaTime)
{
	return S_OK;
}

HRESULT CBaseAI_Shooter::Do_Attack(const _float& fDeltaTime)
{
	return S_OK;
}

CBaseAI_Shooter* CBaseAI_Shooter::Create(_Device pDevice)
{
	return nullptr;
}

Engine::CGameObject* CBaseAI_Shooter::Clone(void* pArg)
{
	return nullptr;
}

void CBaseAI_Shooter::Free()
{
	CBaseAI::Free();
}
