#include "MemeoryPoolManager.h"
#include "MemoryPool.h"
#include "GameObject.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMemeoryPoolManager)

CMemeoryPoolManager::CMemeoryPoolManager()
{
}

Engine::CMemoryPool* CMemeoryPoolManager::Get_MemoryPool(const std::wstring& poolName)
{
	auto iter = m_pMemoryPool.find(poolName);
	if (iter == m_pMemoryPool.end())
	{
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}

HRESULT CMemeoryPoolManager::Add_MemoryPool(const std::wstring& poolName, CMemoryPool* pPool)
{
	auto iter = m_pMemoryPool.find(poolName);
	if (iter == m_pMemoryPool.end())
	{
		m_pMemoryPool.insert(make_pair(poolName,pPool));

		return S_OK;
	}
	else
	{
		Safe_Release(pPool);

		return E_FAIL;
	}	
}

Engine::CGameObject* CMemeoryPoolManager::Get_Data_From_MemoryPool(const std::wstring& poolName)
{
	return Get_MemoryPool(poolName)->Get_Object();
}

HRESULT CMemeoryPoolManager::Release_Data_To_MemoryPool(const std::wstring& poolName, CGameObject* pData)
{
	return Get_MemoryPool(poolName)->Release_Object(pData);
}

void CMemeoryPoolManager::Free(void)
{
	for (auto& Pair : m_pMemoryPool)
	{
		Safe_Release(Pair.second);
	}
	m_pMemoryPool.clear();
}
