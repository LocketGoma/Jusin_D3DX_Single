#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include "Engine_Include.h"

class ENGINE_DLL CBase
{
protected:
	explicit CBase();
	virtual ~CBase();

public:
	_ulong AddRef();
	_ulong Release();

	virtual void Free() PURE;

private:
	_ulong	m_dwRefCount;	
};


#endif // !__BASE_H__