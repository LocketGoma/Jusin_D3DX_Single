#pragma once

#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "Scene.h"

//로딩 씬
//씬 전환시마다 삽입되어서 돌아감.

class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene(_Device pDevice, ESceneType eSceneID);
	virtual ~CLoadingScene() = default;

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

public:
	static unsigned _stdcall LoadingByThread(void* pParam);
	_bool IsFinished();

private:
	HRESULT Load_Base_Resource();
	HRESULT Load_Stage1_Resource();
	virtual void Free() override;

public:
	static CLoadingScene* Create(_Device pDevice, ESceneType eSceneID);

private:
	HANDLE m_hLoadingThread;
	CRITICAL_SECTION m_pCritSection;

	ESceneType m_eNextLoadScene;
	_bool m_bLoadFinished = false;

};


#endif // !__LOADING_SCENE_H__