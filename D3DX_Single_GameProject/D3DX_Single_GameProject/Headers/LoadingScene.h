#pragma once

#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "Scene.h"

//�ε� ��
//�� ��ȯ�ø��� ���ԵǾ ���ư�.

class CLoadingScene : public Engine::CScene
{
private:
	explicit CLoadingScene(_Device pDevice, LOADINGID eSceneID);
	virtual ~CLoadingScene() = default;

//�⺻
public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

//Getter
public:
	LOADINGID	Get_LoadingID() const;
	CRITICAL_SECTION* Get_CriticalSection();
	const _tchar* Get_String() const;

//�ε� ����
public:
	HRESULT	Ready_Loading(LOADINGID eLoadingID);
	

	static unsigned __stdcall LoadingByThread(void* pParam);
	_bool IsFinished();

private:
	HRESULT Load_Base_Resource();
	HRESULT Load_Stage1_Resource();
	virtual void Free() override;

public:
	static CLoadingScene* Create(_Device pDevice, LOADINGID eSceneID);

private:
	HANDLE				m_hLoadingThread;
	CRITICAL_SECTION	m_pCritSection;

	LOADINGID			m_eNextLoadScene;
	_bool				m_bLoadFinished = false;

	_tchar				m_szString[256];

};


#endif // !__LOADING_SCENE_H__