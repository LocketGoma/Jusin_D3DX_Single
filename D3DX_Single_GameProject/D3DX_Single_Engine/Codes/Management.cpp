#include "..\Headers\Management.h"

USING(Engine)

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
	: m_pDeviceManager(CGraphicDevice::Get_Instance())
	, m_pRenderer(CRenderer::Get_Instance())
	, m_pPrototypeManager(CPrototypeManager::Get_Instance())
	, m_pGraphicManager(CGraphicResourceManager::Get_Instance())
	, m_pKeyManager(CKeyManager::Get_Instance())
	, m_pSceneManager(CSceneManager::Get_Instance())
	, m_pGameObjectManager(CGameObjectManager::Get_Instance())
	, m_pTimeManager(CTimeManager::Get_Instance())
	//, m_pSound_Manager(CSoundManager::Get_Instance())

{
	Safe_AddReference(m_pDeviceManager);
	Safe_AddReference(m_pRenderer);
	Safe_AddReference(m_pPrototypeManager);
	Safe_AddReference(m_pGraphicManager);
	Safe_AddReference(m_pKeyManager);
	Safe_AddReference(m_pSceneManager);
	Safe_AddReference(m_pGameObjectManager);
	Safe_AddReference(m_pTimeManager);
	//Safe_AddReference(m_pSound_Manager);
}


HRESULT CManagement::Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, WINMODE eDisplayMode, _uint iSceneCount)
{
	if (nullptr == hWnd ||
		nullptr == m_pDeviceManager ||
		nullptr == m_pRenderer ||
		nullptr == m_pKeyManager ||
		nullptr == m_pTimeManager ||
		nullptr == m_pGameObjectManager)
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
	return m_pSceneManager->LateUpdate_Scene(fDeltaTime);
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

HRESULT CManagement::Setup_SceneManager(_int iMaxSceneIndex)
{
	return m_pSceneManager->Setup_SceneManager(iMaxSceneIndex);
}

HRESULT CManagement::Setup_CurrentScene(_int iSceneIndex, CScene* pCurrentScene)
{
	return m_pSceneManager->Setup_CurrentScene(iSceneIndex, pCurrentScene);
}

CScene* CManagement::Get_NowScene()
{
	return m_pSceneManager->Get_NowScene();
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

HRESULT CManagement::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	if (m_pPrototypeManager == nullptr)
	{
		return E_FAIL;
	}

	return m_pPrototypeManager->Ready_Prototype(pProtoTag,pInstance);
}

CComponent* CManagement::Clone_Prototype(const _tchar* pProtoTag)
{
	if (m_pPrototypeManager == nullptr)
	{
		return nullptr;
	}

	return m_pPrototypeManager->Clone_Prototype(pProtoTag);
}

HRESULT CManagement::Reserve_Size(const _uint& wSize)
{
	return m_pGraphicManager->Reserve_Size(wSize);
}

HRESULT CManagement::Ready_Buffer(_Device pDevice, const _uint& iIndex, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
	return m_pGraphicManager->Ready_Buffer(pDevice,iIndex,pBufferTag,eID,dwCountX,dwCountZ,dwVTXInterval);
}

HRESULT CManagement::Ready_Texture(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	return m_pGraphicManager->Ready_Texture(pDevice, wContainerIdx,  pTextureTag,eType, pPath, iCnt);
}

HRESULT CManagement::Ready_Meshes(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	return m_pGraphicManager->Ready_Meshes(pDevice,wContainerIdx,pMeshTag,eType,pFilePath,pFileName);
}
//툴용
HRESULT CManagement::Ready_Meshes_Force(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	return m_pGraphicManager->Ready_Meshes_Force(pDevice, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}

CComponent* CManagement::Clone_Resource(const _uint& iIndex, const _tchar* pResourceTag)
{
	return m_pGraphicManager->Clone_Resource(iIndex,pResourceTag);
}

HRESULT CManagement::Add_GameObject_Prototype(const std::wstring& PrototypeTag, CGameObject* pPrototype)
{
	if (pPrototype == nullptr)
	{
		return E_FAIL;
	}

	return m_pGameObjectManager->Add_GameObject_Prototype(PrototypeTag,pPrototype);
}

CGameObject* CManagement::Clone_GameObject(const std::wstring& PrototypeTag, void* pArg)
{
	return m_pGameObjectManager->Clone_GameObject(PrototypeTag, pArg);
}

void CManagement::Free()
{
	//Safe_Release(m_pSoundManager);
	Safe_Release(m_pTimeManager);
	Safe_Release(m_pKeyManager);
	Safe_Release(m_pSceneManager);
	Safe_Release(m_pGameObjectManager);
	Safe_Release(m_pGraphicManager);
	Safe_Release(m_pPrototypeManager);
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
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CTime_Manager (Management.cpp)");

	if (CSceneManager::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CSceneManager (Management.cpp)");

	if (CGameObjectManager::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CGameObjectManager (Management.cpp)");

	if (CKeyManager::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CKeyManager (Management.cpp)");

	if (CGraphicResourceManager::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CGraphicResourceManager (Management.cpp)");

	if (CPrototypeManager::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CProtoTypeManager (Management.cpp)");

	if (CRenderer::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CRenderer");

	if (CGraphicDevice::Destroy_Instance())
		PRINT_LOG(L"FATAL ERROR", L"Failed To Release CGraphicDevice");

}
