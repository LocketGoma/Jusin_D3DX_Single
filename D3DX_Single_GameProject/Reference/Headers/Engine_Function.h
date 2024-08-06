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

template <typename T>
bool isEqual(T& p1, T& p2, unsigned int iLevel = 1)
{
	T p3 = p1 - p2;
	double Epsilons = 0.1;
	for (int i = 1; i < iLevel; i++)
	{
		Epsilons *= 0.1;
	}

	if (p3 > -(T)Epsilons && p3 < (T)Epsilons)
		return true;
	else
		return false;


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

		if (Pair.second != nullptr)
		dwRefCnt = Pair.second->Release();

		if (0 == dwRefCnt)
			Pair.second = NULL;
	}
};

//태그 검색용 함수 객체
class CTag_Finder
{
public:
	explicit CTag_Finder(const _tchar* pTag)
		: m_pTargetTag(pTag)
	{
	}
	~CTag_Finder() {		}
public:
	template<typename T>
	bool operator()(const T& pair)
	{
		if (0 == lstrcmpW(m_pTargetTag, pair.first))
		{
			return true;
		}

		return false;
	}

private:
	const _tchar* m_pTargetTag = nullptr;
};

#endif