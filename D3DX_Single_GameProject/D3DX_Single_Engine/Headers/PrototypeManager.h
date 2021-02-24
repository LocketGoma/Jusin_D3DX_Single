#pragma once

#ifndef __PROTOTYPE_MANAGER_H__
#define __PROTOTYPE_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

//프로토타입 매니저...
class ENGINE_DLL CPrototypeManager : public CBase
{
	DECLARE_SINGLETON(CPrototypeManager)

private :
	explicit CPrototypeManager();
	virtual ~CPrototypeManager() = default;

public:
	HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
	CComponent* Clone_Prototype(const _tchar* pProtoTag);

private:
	CComponent* Find_Prototype(const _tchar* pProtoTag);

private:
	std::map<const _tchar*, CComponent*>		m_mapProto;

public:
	virtual void Free(void);

};

END_NAMESPACE

#endif // !__PROTOTYPE_MANAGER_H__