#pragma once

#ifndef __MAIN_STAGE_A_H__
#define __MAIN_STAGE_A_H__

#include "Scene.h"
#include "Base.h"

class CNaviMeshController;
class CMainStageA : public Engine::CScene
{
private:
	explicit CMainStageA(_Device pDevice);
	virtual ~CMainStageA() = default;


public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

private:
	HRESULT Ready_Resource(_Device& pDevice);

private:
	HRESULT Add_Player_Layer(const _tchar* pLayerTag);		//카메라 레이어 포함
	HRESULT Add_Enemy_Layer(const _tchar* pLayerTag);
	HRESULT Add_Boss_Layer(const _tchar* pLayerTag);
	HRESULT Add_Object_Layer(const _tchar* pLayerTag);
	HRESULT Add_ColiderBox_Layer(const _tchar* pLayerTag);
	HRESULT Add_Environment_Layer(const _tchar* pLayerTag);
	HRESULT Add_Weapon_Layer(const _tchar* pLayerTag);

	//개미귀신 2차 스폰, 3차 스폰 용
	HRESULT Add_Enemy_Layer_WaveB(const _tchar* pLayerTag);
	HRESULT Add_Enemy_Layer_WaveC(const _tchar* pLayerTag);

	void Change_Scene(ESceneType eSceneType);

public:
	static CMainStageA* Create(_Device pDevice);

private:
	virtual void Free() override;

	CNaviMeshController* m_pNaviController;

};


#endif // !__MAIN_STAGE_A_H__