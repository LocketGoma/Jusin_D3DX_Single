#include "..\Headers\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(const std::wstring& pObjectTag, CGameObject* pGameObject)
{
	if (pObjectTag.length() < 2 || pGameObject == nullptr)
		return E_FAIL;
	if (m_mapObject.find(pObjectTag) != m_mapObject.end())
		return E_FAIL;
	

	m_mapObject.emplace(pObjectTag, pGameObject);

	return S_OK;
}

CGameObject* CLayer::Find_GameObject(const std::wstring& pObjectTag)
{
	auto iter = m_mapObject.find(pObjectTag);

	if (iter == m_mapObject.end())
	{
		return nullptr;
	}

	return iter->second;
}

CGameObject* CLayer::Get_GameObject(const std::wstring& pObjectTag)
{
	return Find_GameObject(pObjectTag);
}

CComponent* CLayer::Get_Component(const std::wstring& pObjectTag, const std::wstring& pComponentTag, COMPONENT_ID eID)
{
	auto	iter = m_mapObject.find(pObjectTag);

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

std::map<const std::wstring, CGameObject*>* CLayer::Get_ObjectLayer()
{
	return &m_mapObject;
}

_uint CLayer::Get_ObjectCount()
{
	return m_mapObject.size();
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fDeltaTime)
{
	for (auto& iter = m_mapObject.begin() ; iter!=m_mapObject.end(); )
	{
		iter->second->Update_GameObject(fDeltaTime);

		if (iter->second->Is_Dead())
		{
			Safe_Release(iter->second);
			m_mapObject.erase(iter->first);
			iter = m_mapObject.begin();
		}
		else
		{
			iter++;
		}
	}

	return 0;
}

_int CLayer::LateUpdate_Layer(const _float& fDeltaTime)
{
	for (auto& iter : m_mapObject)
	{
		iter.second->LateUpdate_GameObject(fDeltaTime);
	}

	return 0;
}

void CLayer::Render_Layer(void)
{
	for (auto& iter : m_mapObject)
	{
		iter.second->Render_GameObject();
	}
}

CLayer* CLayer::Create(void)
{
	CLayer* pInstance = new CLayer;

	if (FAILED(pInstance->Ready_Layer()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free(void)
{
	for_each(m_mapObject.begin(), m_mapObject.end(), CDeleteMap());	
	m_mapObject.clear();
}
