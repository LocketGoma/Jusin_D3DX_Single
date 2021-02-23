#pragma once
#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)



class CSceneManager final : public CBase
{
	DECLARE_SINGLETON(CSceneManager)

private:
	CSceneManager();
	virtual ~CSceneManager() = default;

public:
	HRESULT Setup_SceneManager(_int iMaxSceneIndex);
	HRESULT Setup_CurrentScene(_int iSceneIndex, class CScene* pCurrentScene);
	_uint Update_Scene(_float fDeltaTime);
	_uint LateUpdate_Scene(_float fDeltaTime);
	HRESULT Render_Scene(HWND hWnd);

public:
	virtual void Free() override;

private:
	class CScene* m_pCurrentScene = nullptr;
	_int m_iCurrentSceneIndex = -1;
	_int m_iNextSceneIndex = -1;
	_int m_iMaxSceneIndex = 0;

};

END_NAMESPACE

#endif