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


#define NULL_CHECK( _ptr)	\
	{if( _ptr == 0){__asm { int 3 };return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
	{if( _ptr == 0){__asm { int 3 };return _return;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
	{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);__asm { int 3 };return _return;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
	{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); __asm { int 3 }; return _return;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
	{ MessageBoxW(NULL, _message, L"System Message",MB_OK); __asm { int 3 };return _return;}


#define __ENGINE_DEFINE_H__
#endif