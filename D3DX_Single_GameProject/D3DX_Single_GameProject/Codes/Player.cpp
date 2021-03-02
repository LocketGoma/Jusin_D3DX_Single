#include "framework.h"


#include "VTXTriColor.h"
#include "Transform.h"
#include "Player.h"

CPlayer::CPlayer(_Device pDevice)
	: Engine::CGameObject(pDevice)
{
}

HRESULT CPlayer::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


	//m_pTransformCom->Set_Scale(_float3(0.1f, 0.1f, 0.1f));

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	Key_Input(fTimeDelta);

	return NO_EVENT;
}

_int CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	//update-transform

	m_pTransformCom->Update_Component(fTimeDelta);

	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

		
	return NO_EVENT;
}

HRESULT CPlayer::Render_GameObject(void)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pTransformCom->LateUpdate_Component(0.f);

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
		return MANAGER_OUT;
	}
	
	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXTriColor*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_TriColor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Buffer", pComponent);

	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_Transform", pComponent);


	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}

	if (pManagement->Key_Pressing(VK_UP))
	{

		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, fTimeDelta);
	}

	if (pManagement->Key_Pressing(VK_DOWN))
	{
		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, -fTimeDelta);
	}

	if (pManagement->Key_Pressing(VK_LEFT))
	{

		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, -fTimeDelta);
	}
	if (pManagement->Key_Pressing(VK_RIGHT))
	{		
		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, fTimeDelta);
	}

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
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();
}
