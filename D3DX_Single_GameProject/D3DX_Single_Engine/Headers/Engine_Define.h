#pragma once
#ifndef __ENGINE_DEFINE_H__

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif

#define PRINT_LOG(Caption, Message)	\
::MessageBox(0, Message, Caption, MB_OK)

//헷깔리게 begin / end 쓰지말고 namespace라고 명시해주기로 함.
#define BEGIN_NAMESPACE(Name) namespace Name {
#define END_NAMESPACE }

#define WINCX 1280
#define WINCY 720

#define USING(Name) using namespace Name;

#define NO_COPY(ClassName)						\
ClassName(const ClassName&)	= delete;			\
ClassName& operator=(const ClassName&) = delete;

#define DECLARE_SINGLETON(ClassName)			\
		NO_COPY(ClassName)						\
public:											\
	static ClassName* Get_Instance();			\
	static _uint Destroy_Instance();			\
private:										\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)			\
ClassName* ClassName::m_pInstance = nullptr;	\
ClassName* ClassName::Get_Instance()			\
{												\
	if (nullptr == m_pInstance)					\
		m_pInstance = new ClassName;			\
	return m_pInstance;							\
}												\
_uint ClassName::Destroy_Instance()				\
{												\
	_uint iRefCount = 0;						\
	if (m_pInstance)							\
	{											\
	iRefCount = m_pInstance->Release();			\
	}											\
	return iRefCount;							\
}

#define __ENGINE_DEFINE_H__
#endif