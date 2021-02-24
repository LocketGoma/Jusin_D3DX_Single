#pragma once

#ifndef __GAMEOBJECT_MANAGER_H__
#define __GAMEOBJECT_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"
#include "GameObject.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGameObjectManager : public CBase
{
	DECLARE_SINGLETON(CGameObjectManager)

private:
	explicit CGameObjectManager();
	virtual ~CGameObjectManager() = default;

public:
	virtual void Free(void);

};


END_NAMESPACE


#endif