#include "..\Headers\Base.h"

USING(Engine)

CBase::CBase()
    : m_dwRefCount(0)
{
}

CBase::~CBase()
{
}

_ulong CBase::AddRef()
{
    return ++m_dwRefCount;
}

_ulong CBase::Release()
{
	if (0 == m_dwRefCount)
	{
		Free();

#ifdef _DEBUG
		delete this;
#endif // _DEBUG

		return 0;
	}
	else 
	{
		return m_dwRefCount--;
	}
}
