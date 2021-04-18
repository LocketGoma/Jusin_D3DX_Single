#include "MemoryPool.h"
#include "GameObject.h"

USING(Engine)


CMemoryPool::CMemoryPool(CGameObject* pOriginal)
	: m_pOrginal(nullptr)
	, m_iAllockCount(0)
	, m_iReadyCount(0)
	, m_iTotalCount(0)
{
	if (pOriginal == nullptr)
	{
		PRINT_LOG(L"FATAL_ERROR", L"MemoryPool create fail");
	}

	m_pOrginal = pOriginal;

	m_pFirstNode = new NODE;
	m_pFirstNode->pNextBlock = nullptr;
	m_pFirstNode->pData = m_pOrginal;

}


Engine::CGameObject* CMemoryPool::Get_Object()
{
	if (m_iReadyCount == 0)
	{
		NODE* pNextNode = new NODE;
		pNextNode->pNextBlock = nullptr;
		pNextNode->pData = m_pOrginal->Clone();
		
		m_pFirstNode->pNextBlock = pNextNode;


		m_iReadyCount++;
		m_iTotalCount++;
	}
	NODE* pReturnNode = m_pFirstNode->pNextBlock;
	if (pReturnNode == nullptr)
	{
		return nullptr;
	}
	CGameObject* pReturn = pReturnNode->pData;
	pReturnNode->pData = nullptr;
	
	m_pFirstNode->pNextBlock = m_pFirstNode->pNextBlock->pNextBlock;

	NodeClear(pReturnNode);

	//1. 시작 노드의 다음 노드 데이터 값을 리턴 대기
	//2. 시작노드의 다음 노드를 "리턴 될 노드의 다음 노드"로 변경
	//3. 노드 리턴

	m_iReadyCount--;
	m_iAllockCount++;

	return pReturn;
}


HRESULT CMemoryPool::Release_Object(CGameObject* pData)
{
	if (pData == nullptr)
	{
		return E_FAIL;
	}

	NODE* pNode = new NODE;
	pNode->pNextBlock = m_pFirstNode->pNextBlock;
	pNode->pData = pData;
	m_pFirstNode->pNextBlock = pNode;

	m_iReadyCount++;
	m_iAllockCount--;

	return S_OK;
}


void CMemoryPool::NodeClear(NODE* pNode)
{
	pNode->pNextBlock = nullptr;
	if (pNode->pData != nullptr)
	{
		Safe_Release(pNode->pData);
	}
	delete pNode;
	pNode = nullptr;
}


void CMemoryPool::Free()
{
	NODE* pNextNode = m_pFirstNode;
	while (true)
	{
		if (pNextNode == nullptr)
		{
			break;
		}
		m_iReadyCount--;

		NODE* pNT = pNextNode->pNextBlock;

		pNextNode->pNextBlock = nullptr;
		Safe_Release(pNextNode->pData);

		delete pNextNode;
		pNextNode = pNT;

	}
	if (m_iReadyCount != -1)		//First도 지워서 그럼.
	{
		PRINT_LOG(L"FATAL_ERROR", L"MemoryPool Release Fail");
	}

}
