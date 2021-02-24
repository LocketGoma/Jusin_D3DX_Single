#include "framework.h"


#include "VTXTriColor.h"
#include "Transform.h"
#include "..\Headers\Player.h"

CPlayer::CPlayer(_Device pDevice)
	: Engine::CGameObject(pDevice)
{
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	return 0;
}

_int CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	//update-transform

	//m_pTransformCom->Update_Component(fTimeDelta);

	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);


	return _int();
}

HRESULT CPlayer::Render_GameObject(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pBufferCom->Render_Buffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return 0;
	}
	
	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXTriColor*>(pManagement->Clone_Resource(0, L"Buffer_TriColor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free(void)
{


	Engine::CGameObject::Free();
}
