#include "SceneManager.h"
#include "Scene.h"

#include "GameObject.h"

USING(Engine)
IMPLEMENT_SINGLETON(CSceneManager)

CSceneManager::CSceneManager()
	: m_iMaxSceneIndex(0)
{
}

HRESULT CSceneManager::Setup_SceneManager(_int iMaxSceneIndex)
{
	if (iMaxSceneIndex < 1)
	{
		return E_FAIL;
	}

	m_iMaxSceneIndex = iMaxSceneIndex;

	return S_OK;
}

HRESULT CSceneManager::Setup_CurrentScene(_int iSceneIndex, CScene* pCurrentScene)
{
	if (m_iMaxSceneIndex == 0)
	{
		return E_FAIL;
	}
	if (nullptr == pCurrentScene)
		return E_FAIL;

	//바뀔 씬이 다르다면 현재 씬을 릴리즈 해주고 진행
	if (m_iCurrentSceneIndex != iSceneIndex)
	{
		Safe_Release(m_pCurrentScene);
		m_pCurrentScene = pCurrentScene;

		m_iCurrentSceneIndex = iSceneIndex;

		m_bChangeTrigger = true;
	}
	else
	{
		Safe_Release(pCurrentScene);
	}

	return S_OK;
}

CScene* CSceneManager::Get_NowScene()
{
	return m_pCurrentScene;
}

_uint CSceneManager::Update_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene)
	{
		return 0;
	}
	m_bChangeTrigger = false;

	return m_pCurrentScene->Update_Scene(fDeltaTime);
}

_uint CSceneManager::LateUpdate_Scene(_float fDeltaTime)
{
	if (nullptr == m_pCurrentScene || m_bChangeTrigger == true)
	{
		return 0;
	}

	return m_pCurrentScene->LateUpdate_Scene(fDeltaTime);
}

void CSceneManager::Render_Scene()
{
	if (nullptr == m_pCurrentScene || m_bChangeTrigger == true)
	{
		return;
	}
	m_pCurrentScene->Render_Scene();
}

CGameObject* CSceneManager::Get_GameObject_From_Layer(const std::wstring& pLayerTag, const std::wstring& pObjectTag)
{
	if (m_pCurrentScene != nullptr)
	{
		CLayer* pLayer = m_pCurrentScene->Get_Layer(pLayerTag.c_str());

		if (pLayer == nullptr)
		{
			return nullptr;
		}
		else
		{
			return pLayer->Find_GameObject(pObjectTag.c_str());
		}		
	}
	
	return nullptr;
}



void CSceneManager::Free()
{
	Safe_Release(m_pCurrentScene);
}
