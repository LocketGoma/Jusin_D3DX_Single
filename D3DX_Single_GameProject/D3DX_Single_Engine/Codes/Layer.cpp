#include "..\Headers\Layer.h"

USING(Engine)

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(const _tchar* pObjectTag, CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return E_FAIL;

	m_mapObject.emplace(pObjectTag, pGameObject);

	return S_OK;
}

CGameObject* CLayer::Find_GameObject(const _tchar* pObjectTag)
{
	auto iter = m_mapObject.find(pObjectTag);

	if (iter == m_mapObject.end())
	{
		return nullptr;
	}

	return iter->second;
}

CGameObject* CLayer::Get_GameObject(const _tchar* pObjectTag)
{
	return Find_GameObject(pObjectTag);
}

CComponent* CLayer::Get_Component(const _tchar* pObjectTag, const _tchar* pComponentTag, COMPONENT_ID eID)
{
	auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjectTag));

	if (iter == m_mapObject.end())
		return nullptr;

	return iter->second->Get_Component(pComponentTag, eID);
}

std::map<const _tchar*, CGameObject*>* CLayer::Get_ObjectLayer()
{
	return &m_mapObject;
}

HRESULT CLayer::Ready_Layer(void)
{
	return S_OK;
}

_int CLayer::Update_Layer(const _float& fDeltaTime)
{
	for (auto& iter : m_mapObject)
	{
		iter.second->Update_GameObject(fDeltaTime);
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
