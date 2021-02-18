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

private:
	HRESULT Setup_DefaultSetting();
	HRESULT Ready_Scene();


public:
	static CMainGame* Create();
	virtual void Free() override;

private:
	Engine::CManagement* m_pManagement = nullptr;

	_Device			m_pDevice = nullptr;

};


#endif // !__MAINAPP_H__