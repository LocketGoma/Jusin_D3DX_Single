#include "SceneManager.h"
#include "Scene.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
{
}

HRESULT CSceneManager::Setup_CurrentScene(_int iSceneIndex, CScene* pCurrentScene)
{
	if (nullptr == pCurrentScene)
		return E_FAIL;

	if (m_iCurrentSceneIndex != iSceneIndex)
	{
		Safe_Release(m_pCurrentScene);
		m_pCurrentScene = pCurrentScene;

		m_iCurrentSceneIndex = iSceneIndex;
	}
	else
	{
		Safe_Release(pCurrentScene);
	}

	return S_OK;
}

_uint CSceneManager::Update_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->Update_Scene(fDeltaTime);
}

_uint CSceneManager::LateUpdate_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
		return 0;

	return m_pCurrentScene->LateUpdate_Scene(fDeltaTime);
}

void CSceneManager::Free()
{
	Safe_Release(m_pCurrentScene);
}
