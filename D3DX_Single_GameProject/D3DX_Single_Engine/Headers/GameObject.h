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
	_float Get_ViewZ();
	CComponent* Get_Component(const std::wstring& pComponentTag, COMPONENT_ID eID);
	CComponent* Find_Component(const std::wstring& pComponentTag, COMPONENT_ID eID);

public:
	virtual HRESULT Ready_GameObject();
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr);
	virtual _int Update_GameObject(const _float& fDeltaTime);
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime);
	virtual HRESULT Render_GameObject();

public:
	virtual void Set_Position(_vec3 vPos) PURE;
	virtual void Set_Size(_vec3 vSize) PURE;
	virtual _vec3 Get_Position() PURE;
	virtual _vec3 Get_Size() PURE;

protected:
	_bool m_bIsPrototype;
	_float m_fViewZ = 0.f;						//Z소팅 연산용
	_Device m_pDevice;
	std::map<const std::wstring, CComponent*> m_mapComponent[(_uint)(COMPONENT_ID::ID_END)];

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void	Free(void);

};

END_NAMESPACE

#endif // !__GAMEOBJECT_H__