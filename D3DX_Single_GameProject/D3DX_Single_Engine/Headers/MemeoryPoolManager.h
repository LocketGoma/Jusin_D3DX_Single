#pragma once

#ifndef __MEMORYPOOL_MANAGER_H__
#define __MEMORYPOOL_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"
#include "MemoryPool.h"

//메모리풀 생성/해제 관리 클래스

class CMemoryPool;
class CGameObject;

BEGIN_NAMESPACE(Engine)


class ENGINE_DLL CMemeoryPoolManager : public CBase
{
	DECLARE_SINGLETON(CMemeoryPoolManager)

private:
	explicit CMemeoryPoolManager();
	virtual ~CMemeoryPoolManager() = default;

public:
	CMemoryPool* Get_MemoryPool(const std::wstring& poolName);
	HRESULT Add_MemoryPool(const std::wstring& poolName, CMemoryPool* pPool);

	CGameObject* Get_Data_From_MemoryPool(const std::wstring& poolName);
	HRESULT Release_Data_To_MemoryPool(const std::wstring& poolName, CGameObject* pData);


public:
	virtual void Free(void);

private:
	typedef std::map<std::wstring, CMemoryPool*> POOL;
	POOL m_pMemoryPool;



};

END_NAMESPACE

#endif // !__MEMORYPOOL_MANAGER_H__