#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Layer.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(_Device pDevice);
	virtual ~CScene();

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

	std::map<const std::wstring, CLayer*>* Get_LayerList();
	CLayer* Get_Layer(const std::wstring& pLayerName);

	CGameObject* Find_GameObject(const std::wstring& pLayerName, const std::wstring& pObjectTag);

protected:
	_Device									m_pDevice;
	std::map<const std::wstring, CLayer*>	m_mapLayer;

public:
	virtual void	Free(void);

};

END_NAMESPACE

#endif // !__SCENE_H__