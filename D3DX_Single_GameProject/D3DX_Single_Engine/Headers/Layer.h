#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "GameObject.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public :
	HRESULT Add_GameObject(const _tchar* pObjectTag, CGameObject* pGameObject);
	CGameObject* Find_GameObject(const _tchar* pObjectTag);
	CGameObject* Get_GameObject(const _tchar* pObjectTag);
	CComponent* Get_Component(const _tchar* pObjectTag, const _tchar* pComponentTag, COMPONENT_ID eID);
	std::map<const _tchar*, CGameObject*>* Get_ObjectLayer();

public :
public:
	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fDeltaTime);
	virtual _int		LateUpdate_Layer(const _float& fDeltaTime);
	virtual void		Render_Layer(void);

private:
	std::map<const _tchar*, CGameObject*>		m_mapObject;

public:
	static CLayer* Create(void);
	virtual void	Free(void);


};

END_NAMESPACE

#endif // !__LAYER_H__