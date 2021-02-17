#pragma once

#ifndef __MANAGEMENT_H__
#define __MANAGEMENT_H__

#include "Engine_Include.h"
#include "Base.h"

#include "GraphicDevice.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "Renderer.h"


BEGIN_NAMESPACE(Engine)
//마스터 클래스
class ENGINE_DLL CManagement final : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	CManagement();
	virtual ~CManagement() = default;

	
public: 
	//base Setting
	HRESULT Ready_Engine(HWND hWnd, int iWinCX, int iWinCY, WINMODE eDisplayMode, _uint iSceneCount);
	_uint Update_Engine();
	HRESULT Render_Engine(HWND hWnd = nullptr);

public :
	//GraphicDevice Setting
	_Device	Get_Device();

public :
	//TimeManager Setting
	//HRESULT Ready_Timer();
	//_float Get_DeltaTime();

public :
	//KeyManager Setting
	_bool Key_Pressing(_uint iKey);
	_bool Key_Down(_uint iKey);
	_bool Key_Up(_uint iKey);	 

public: /*For.Renderer*/
	HRESULT Add_RenderList(RENDERID eRenderID, class CGameObject* pGameObject);


public:
	virtual void Free() override;
	static void Release_Engine();

private:
	CGraphicDevice* m_pDeviceManager	= nullptr;
	CRenderer*		m_pRenderer			= nullptr;
	CKeyManager*	m_pKeyManager		= nullptr;
	CTimeManager*	m_pTimeManager		= nullptr;




};

END_NAMESPACE

#endif // !__MANAGEMENT_H__