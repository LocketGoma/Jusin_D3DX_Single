#pragma once

#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected :
	explicit CGameObject(_Device pDevice);
	explicit CGameObject(const CGameObject& other);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Ready_GameObject();
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime);
	virtual void Render_GameObject();

protected:
	_Device m_pDevice;
	std::map<const _tchar*, CComponent*> m_mapComponent[(_uint)(COMPONENT_ID::ID_END)];

public:
	virtual void	Free(void);
};

END_NAMESPACE

#endif // !__GAMEOBJECT_H__