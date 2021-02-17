#pragma once
#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

template <typename T>
void Safe_Delete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void Safe_Delete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

template <typename T>
_uint Safe_AddReference(T& ptr)
{
	_uint iRefCount = 0;

	if (ptr)
	{
		iRefCount = ptr->AddRef();
	}

	return iRefCount;
}

template <typename T>
_uint Safe_Release(T& ptr)
{
	_uint iRefCount = 0;
	if (ptr)
	{
		iRefCount = ptr->Release();
		if (iRefCount == 0)
		{
			ptr = nullptr;
		}
	}
	return iRefCount;
}

//Map 연관 컨테이너 삭제용 함수 객체
class CDeleteMap
{
public:
	explicit CDeleteMap(void) {}
	~CDeleteMap(void) {}
public: // operator	
	template <typename T>
	void operator () (T& Pair)
	{
		_ulong dwRefCnt = 0;

		dwRefCnt = Pair.second->Release();

		if (0 == dwRefCnt)
			Pair.second = NULL;
	}
};

#endif