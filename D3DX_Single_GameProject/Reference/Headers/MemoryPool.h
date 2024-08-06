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
	CMemoryPool(CGameObject* pOriginal);				//���� �Ҵ�
	virtual ~CMemoryPool() = default;
	
//�Ҵ� / ����
	virtual CGameObject* Get_Object();
	virtual HRESULT Release_Object(CGameObject* pData);

public:
	// CBase��(��) ���� ��ӵ�
	/// - ���α׷� ������
	virtual void NodeClear(NODE* pNode);

	virtual void Free();

//���� ����
private:
	CGameObject* m_pOrginal;							//��������

	NODE* m_pFirstNode;


	int m_iTotalCount;						//�� �?
	int m_iAllockCount;						//������ΰ� �?
	int m_iReadyCount;						//������ΰ� �?


};

END_NAMESPACE

#endif // !__BASE_MEMORY_POOL_H__