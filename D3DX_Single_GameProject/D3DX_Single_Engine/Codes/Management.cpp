#include "..\Headers\Management.h"

USING(Engine)

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pDeviceManager(CGraphicDevice::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
	//, m_pComponent_Manager(CComponent_Manager::Get_Instance())
	, m_pKeyManager(CKeyManager::Get_Instance())
	, m_pSceneManager(CSceneManager::Get_Instance())
	//, m_pGameObject_Manager(CGameObject_Manager::Get_Instance())
	, m_pTimeManager(CTimeManager::Get_Instance())
	//, m_pSound_Manager(CSoundManager::Get_Instance())

{
	Safe_AddReference(m_pDeviceManager);
	Safe_AddReference(m_pRenderer);
	//Safe_AddReference(m_pComponent_Manager);
	Safe_AddReference(m_pKeyManager);
	Safe_AddReference(m_pSceneManager);
	//Safe_AddReference(m_pGameObject_Manager);
	Safe_AddReference(m_pTimeManager);
	//Safe_AddReference(m_pSound_Manager);
}


HRESULT CManagement::Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, WINMODE eDisplayMode, _uint iSceneCount)
{
	if (nullptr == hWnd ||
		nullptr == m_pDeviceManager ||
		nullptr == m_pRenderer ||
		nullptr == m_pKeyManager ||
		nullptr == m_pTimeManager)
	{
		PRINT_LOG(L"FATAL Error", L"Failed To Ready EngineSystem");
		return E_FAIL;
	}

	if (FAILED(m_pDeviceManager->Ready_GraphicDevice(hWnd, eDisplayMode, iWinCX, iWinCY)))
	{
		PRINT_LOG(L"FATAL Error", L"Failed to Ready_GraphicDevice in Management");
	}

	if (FAILED(m_pRenderer->Set_Device(m_pDeviceManager->GetDevice())))
	{
		PRINT_LOG(L"Error", L"Failed To Set_Device to Renderer");
		return E_FAIL;
	}

	if (FAILED(m_pTimeManager->Ready_TimeManager()))
	{
		PRINT_LOG(L"Error", L"Filed to Set Timer");
		return E_FAIL;
		//뜰 일이 없는 에러
	}


    return S_OK;
}

_uint CManagement::Update_Engine(_float fDeltaTime)
{
	m_pKeyManager->Key_Update();	


	return m_pSceneManager->Update_Scene(fDeltaTime);
}

_uint CManagement::LateUpdate_Engine(_float fDeltaTime)
{
	return m_pSceneManager->Update_Scene(fDeltaTime);
}

HRESULT CManagement::Render_Engine(HWND hWnd)
{
	if (nullptr == m_pRenderer)
		return E_FAIL;

	if (FAILED(m_pRenderer->Render_RenderList(hWnd)))
	{
		PRINT_LOG(L"Error", L"Failed To Render_RenderList");
		return E_FAIL;
	}

	return S_OK;
}

_Device CManagement::Get_Device()
{
	if (nullptr == m_pDeviceManager)
		return nullptr;

	return m_pDeviceManager->GetDevice();
}

_float CManagement::Get_DeltaTime()
{
	return m_pTimeManager->Update_TimeManager();
}

HRESULT CManagement::Setup_CurrentScene(_int iSceneIndex, CScene* pCurrentScene)
{
	return m_pSceneManager->Setup_CurrentScene(iSceneIndex, pCurrentScene);
}

//_uint CManagement::Update_Scene(_float fDeltaTime)
//{
//	if (m_pSceneManager == nullptr)
//	{
//		return E_FAIL;
//	}
//
//	return m_pSceneManager->Update_Scene(fDeltaTime);
//}
//
//_uint CManagement::LateUpdate_Scene(_float fDeltaTime)
//{
//	if (m_pSceneManager == nullptr)
//	{
//		return E_FAIL;
//	}
//
//	return m_pSceneManager->LateUpdate_Scene(fDeltaTime);
//}

_bool CManagement::Key_Pressing(_uint iKey)
{
	return m_pKeyManager->Key_Pressing(iKey);
}

_bool CManagement::Key_Down(_uint iKey)
{
	return m_pKeyManager->Key_Down(iKey);
}

_bool CManagement::Key_Up(_uint iKey)
{
	return m_pKeyManager->Key_Up(iKey);
}

HRESULT CManagement::Add_RenderList(RENDERID eRenderID, CGameObject* pGameObject)
{
	if (m_pRenderer == nullptr)
	{
		return E_FAIL;
	}
	if (pGameObject == nullptr)
	{
		PRINT_LOG(L"Error", L"Can not add Renderlist because GameObject is NULL");
		return E_FAIL;
	}

	return m_pRenderer->Add_RenderList(eRenderID, pGameObject);
}

void CManagement::Free()
{
	//Safe_Release(m_pSoundManager);
	Safe_Release(m_pTimeManager);
	//Safe_Release(m_pGameObjectManager);
	Safe_Release(m_pSceneManager);
	Safe_Release(m_pKeyManager);
	//Safe_Release(m_pComponentManager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDeviceManager);
}

void CManagement::Release_Engine()
{
	/* 항상 CManagement의 Destroy부터 호출해라! */
	if (CManagement::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CManagement");

	//if (CSoundManager::Destroy_Instance())
	//	PRINT_LOG(L"Waring", L"Failed To Release CSoundManager (Management.cpp)");

	if (CTimeManager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CTime_Manager (Management.cpp)");

	//if (CGameObject_Manager::Destroy_Instance())
	//	PRINT_LOG(L"Waring", L"Failed To Release CGameObject_Manager (Management.cpp)");

	if (CSceneManager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CSceneManager (Management.cpp)");

	if (CKeyManager::Destroy_Instance())
		PRINT_LOG(L"Waring", L"Failed To Release CKey_Manager (Management.cpp)");

	//if (CComponent_Manager::Destroy_Instance())
	//	PRINT_LOG(L"Waring", L"Failed To Release CComponent_Manager (Management.cpp)");

	if (CRenderer::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CRenderer");

	if (CGraphicDevice::Destroy_Instance())
		PRINT_LOG(L"Warning", L"Failed To Release CGraphicDevice");

}
