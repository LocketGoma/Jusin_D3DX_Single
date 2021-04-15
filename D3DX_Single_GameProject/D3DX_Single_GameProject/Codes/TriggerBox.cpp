#include "framework.h"
#include "TriggerBox.h"

#include "Collider.h"
#include "Transform.h"

CTriggerBox::CTriggerBox(_Device pDevice)
	: CGameObject(pDevice)
{
}

CTriggerBox::CTriggerBox(const CTriggerBox& other)
	: CGameObject(other)
{
}

HRESULT CTriggerBox::Ready_GameObject(void)
{
	return E_NOTIMPL;
}

HRESULT CTriggerBox::Ready_GameObject_Clone(void* pArg)
{
	return E_NOTIMPL;
}

_int CTriggerBox::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CTriggerBox::LateUpdate_GameObject(const _float& fDeltaTime)
{
	return _int();
}

HRESULT CTriggerBox::Render_GameObject(void)
{
	return E_NOTIMPL;
}

CTriggerBox* CTriggerBox::Create(_Device pDevice)
{
	return nullptr;
}

Engine::CGameObject* CTriggerBox::Clone(void* pArg)
{
	return nullptr;
}

HRESULT CTriggerBox::Add_Component()
{
	return E_NOTIMPL;
}

void CTriggerBox::Free()
{
	Engine::CGameObject::Free();
}
