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
	HRESULT Add_GameObject(const std::wstring& pObjectTag, CGameObject* pGameObject);
	CGameObject* Find_GameObject(const std::wstring& pObjectTag);
	CGameObject* Get_GameObject(const std::wstring& pObjectTag);
	CComponent* Get_Component(const std::wstring& pObjectTag, const std::wstring& pComponentTag, COMPONENT_ID eID);
	std::map<const std::wstring, CGameObject*>* Get_ObjectLayer();
	_uint Get_ObjectCount();
	void Clear_GameObject();

public :
public:
	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fDeltaTime);
	virtual _int		LateUpdate_Layer(const _float& fDeltaTime);
	virtual void		Render_Layer(void);

private:
	std::map<const std::wstring, CGameObject*>		m_mapObject;

public:
	static CLayer* Create(void);
	virtual void	Free(void);


};

END_NAMESPACE

#endif // !__LAYER_H__