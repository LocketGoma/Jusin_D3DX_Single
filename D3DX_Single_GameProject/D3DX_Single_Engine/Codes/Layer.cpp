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
