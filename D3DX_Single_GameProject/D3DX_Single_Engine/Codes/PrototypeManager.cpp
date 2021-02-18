#include "PrototypeManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CPrototypeManager)

CPrototypeManager::CPrototypeManager()
{

}

HRESULT CPrototypeManager::Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	CComponent* pComponent = Find_Prototype(pProtoTag);

	//중복 생성시
	if (NULL != pComponent)
	{
		TCHAR msg[256] = L"";
		TCHAR Omsg[] = L"Prototype < %s > is already Exist";

		swprintf_s(msg, Omsg, pProtoTag);

		PRINT_LOG(L"Warring", msg);
		return S_OK;
	}

	m_mapProto.emplace(pProtoTag, pInstance);

	return S_OK;
}

CComponent* CPrototypeManager::Clone_Prototype(const _tchar* pProtoTag)
{
	CComponent* pComponent = Find_Prototype(pProtoTag);

	if (pComponent == nullptr)
	{
		TCHAR msg[256] = L"";
		TCHAR Omsg[] = L"Component < %s > is not Exist";

		swprintf_s(msg, Omsg, pProtoTag);

		PRINT_LOG(L"Warring", msg);

		return nullptr;
	}

	return pComponent->Clone();
}

CComponent* CPrototypeManager::Find_Prototype(const _tchar* pProtoTag)
{
	auto iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));

	if (iter == m_mapProto.end())
		return nullptr;

	return iter->second;
}

void CPrototypeManager::Free(void)
{
	for_each(m_mapProto.begin(), m_mapProto.end(), CDeleteMap());
	m_mapProto.clear();
}
