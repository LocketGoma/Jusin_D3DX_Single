#include "framework.h"
#include "ProjRocket.h"

HRESULT CProjRocket::Ready_GameObject(_uint iTexNumber)
{
	return E_NOTIMPL;
}

HRESULT CProjRocket::Ready_GameObject_Clone(void* pArg)
{
	return E_NOTIMPL;
}

_int CProjRocket::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CProjRocket::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CProjRocket::Render_GameObject(void)
{
	return E_NOTIMPL;
}

Engine::CGameObject* CProjRocket::Clone(void* pArg)
{
	return nullptr;
}
