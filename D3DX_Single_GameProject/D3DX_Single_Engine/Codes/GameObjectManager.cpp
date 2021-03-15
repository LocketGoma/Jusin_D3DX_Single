#include "GameObjectManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGameObjectManager)

CGameObjectManager::CGameObjectManager()
{

}

CGameObject* CGameObjectManager::Get_GameObject(_uint iSceneIndex, const std::wstring& LayerTag, _uint iIndex)
{
	return nullptr;
}

HRESULT CGameObjectManager::Ready_GameObject_Manager()
{
	if (!m_pGameObject.empty())
	{
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CGameObjectManager::Add_GameObject_Prototype(const std::wstring& PrototypeTag, CGameObject* pPrototype)
{
	if (pPrototype == nullptr)
	{
		TCHAR msg[256] = L"";
		TCHAR Omsg[] = L"Prototype Assign Fail: %s";

		swprintf_s(msg, Omsg, pPrototype);

		PRINT_LOG(L"ERROR", Omsg);

		return E_FAIL;
	}
	auto iter_find = m_pGameObject.find(PrototypeTag);

	if (iter_find == m_pGameObject.end())
	{
		m_pGameObject.insert(make_pair(PrototypeTag, pPrototype));
	}
	else
	{
		Safe_Release(pPrototype);
	}
	return S_OK;
}



CGameObject* CGameObjectManager::Clone_GameObject(const std::wstring& PrototypeTag, void* pArg)
{
	auto iter_find = m_pGameObject.find(PrototypeTag);

	if (iter_find == m_pGameObject.end())
	{
		TCHAR msg[256] = L"";
		TCHAR Omsg[] = L"Can not Find Prototype : %s";

		swprintf_s(msg, Omsg, PrototypeTag);

		PRINT_LOG(L"ERROR", Omsg);

		return nullptr;
	}

	return iter_find->second->Clone(pArg);
}

void CGameObjectManager::Free(void)
{
	for (auto& Pair : m_pGameObject)
	{
		Safe_Release(Pair.second);
	}	

	m_pGameObject.clear();
}
