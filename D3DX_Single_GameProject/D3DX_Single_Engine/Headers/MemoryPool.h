#pragma once

#ifndef __BASE_MEMORY_POOL_H__
#define __BASE_MEMORY_POOL_H__

#include "Base.h"
#include "GameObject.h"

class CGameObject;

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CMemoryPool abstract: public CBase
{
private:
	typedef struct pool_NODE
	{
		pool_NODE* pNextBlock = nullptr;
		
		CGameObject* pData;
	}NODE;

public:
	CMemoryPool(CGameObject* pOriginal);				//원본 할당
	virtual ~CMemoryPool() = default;
	
//할당 / 해제
	virtual CGameObject* Get_Object();
	virtual HRESULT Release_Object(CGameObject* pData);

public:
	// CBase을(를) 통해 상속됨
	/// - 프로그램 해제부
	virtual void NodeClear(NODE* pNode);

	virtual void Free();

//내부 원소
private:
	CGameObject* m_pOrginal;							//오리지널

	NODE* m_pFirstNode;


	int m_iTotalCount;						//총 몇개?
	int m_iAllockCount;						//사용중인거 몇개?
	int m_iReadyCount;						//대기중인거 몇개?


};

END_NAMESPACE

#endif // !__BASE_MEMORY_POOL_H__