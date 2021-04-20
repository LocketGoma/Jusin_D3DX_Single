#pragma once

#ifndef __MAINAPP_H__
#define __MAINAPP_H__

#include "Base.h"
#include "Engine_Include.h"

BEGIN_NAMESPACE(Engine)

class CManagement;

END_NAMESPACE

class CMainGame : public CBase
{
private:
	explicit CMainGame();
	virtual ~CMainGame();

public:
	HRESULT	Ready_MainGame();
	_int	Update_MainGame();	
	void	Render_MainGame();
	_bool	In_TimerLock(_float fDeltaTime);

private:
	HRESULT Setup_DefaultSetting();
	HRESULT Setup_Shader();
	HRESULT Ready_Scene();


public:
	static CMainGame* Create();
	virtual void Free() override;

	static void CALLBACK MouseProc(UINT message, WPARAM wParam);

private:
	Engine::CManagement* m_pManagement = nullptr;

	_uint	m_dwRenderCount = 0;	
	_float m_fAccTimeDelta;
	_float m_fCallLimit;	
	_bool  m_bLock;
	_float m_fTime = 0.f;
	_float m_fFPSTimer = 0.f;
	_Device	m_pDevice = nullptr;

	class Engine::CScene* pScene;

};


#endif // !__MAINAPP_H__