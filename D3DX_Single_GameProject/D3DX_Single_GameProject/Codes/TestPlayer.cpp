#include "framework.h"


#include "VTXTriColor.h"
#include "Transform.h"
#include "TestPlayer.h"
#include "NaviMesh.h"

CTestPlayer::CTestPlayer(_Device pDevice)
	: Engine::CGameObject(pDevice)
{
}

CTestPlayer::CTestPlayer(const CTestPlayer& other)
	: Engine::CGameObject(other)
{
}

HRESULT CTestPlayer::Ready_GameObject(void)
{
	//m_pTransformCom->Set_Scale(_float3(0.1f, 0.1f, 0.1f));

	return S_OK;
}

HRESULT CTestPlayer::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CTestPlayer::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);

	Key_Input(fDeltaTime);

	return NO_EVENT;
}

_int CTestPlayer::LateUpdate_GameObject(const _float& fDeltaTime)
{
	//update-transform

	m_pTransformCom->Update_Component(fDeltaTime);

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

		
	return NO_EVENT;
}

HRESULT CTestPlayer::Render_GameObject(void)
{
	//m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_pNaviMeshCom->Render_NaviMesh(g_bViewNavimesh);

	m_pTransformCom->LateUpdate_Component(0.f);

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	if (FAILED(m_pBufferCom->Render_Buffer()))
		return E_FAIL;

	return S_OK;
}

void CTestPlayer::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CTestPlayer::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Pos(vSize);
}

_vec3 CTestPlayer::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CTestPlayer::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}



HRESULT CTestPlayer::Add_Component(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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


	// NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Navi"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_NAVI", pComponent);

	m_pNaviMeshCom->Add_NaviCell(_vec3(2.0f,0.f,2.f), _vec3(4.0f,0.f,2.0f), _vec3(4.f, 0.f, 0.f) );

	m_pNaviMeshCom->Add_NaviCell(_vec3(4.0f,0.f,2.0f), _vec3(2.0f, 0.f, 2.f), _vec3(4.f, 0.f, 4.f) );

	return S_OK;
}

void CTestPlayer::Key_Input(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return;
	}

	_vec3 vPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	if (pManagement->Key_Pressing(VK_UP))
	{

		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, fDeltaTime);
	}

	if (pManagement->Key_Pressing(VK_DOWN))
	{
		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, -fDeltaTime);
	}

	if (pManagement->Key_Pressing(VK_LEFT))
	{

		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, -fDeltaTime);
	}
	if (pManagement->Key_Pressing(VK_RIGHT))
	{		
		m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, fDeltaTime);
	}
	m_pTransformCom->Update_Component(fDeltaTime);

	_vec3 movePos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);


	m_pTransformCom->Set_Pos(m_pNaviMeshCom->Compare_OnNaviMesh(&vPos, &movePos));

}

CTestPlayer* CTestPlayer::Create(_Device pDevice)
{
	CTestPlayer* pInstance = new CTestPlayer(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CTestPlayer::Clone(void* pArg)
{
	CTestPlayer* pClone = new CTestPlayer(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CTestPlayer");		
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}



	return pClone;
}

void CTestPlayer::Free(void)
{
	Safe_Release(m_pNaviMeshCom);
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();
}
