#pragma once
#ifndef __MAIN_STAGE_C_H__
#define __MAIN_STAGE_C_H__

#include "Scene.h"
#include "Base.h"

class CNaviMeshController;
class CMainStageC : public Engine::CScene
{
private:
	explicit CMainStageC(_Device pDevice);
	virtual ~CMainStageC() = default;


public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

private:
	HRESULT Ready_Resource(_Device& pDevice);

private:
	HRESULT Add_Player_Layer(const _tchar* pLayerTag);		//ī�޶� ���̾� ����
	HRESULT Add_Enemy_Layer(const _tchar* pLayerTag);
	HRESULT Add_Boss_Layer(const _tchar* pLayerTag);
	HRESULT Add_Object_Layer(const _tchar* pLayerTag);
	HRESULT Add_Environment_Layer(const _tchar* pLayerTag);
	HRESULT Add_Weapon_Layer(const _tchar* pLayerTag);


public:
	static CMainStageC* Create(_Device pDevice);

private:
	virtual void Free() override;

	CNaviMeshController* m_pNaviController;
	CNaviMeshController* m_pNaviControllerB;

};


#endif // !__MAIN_STAGE_A_H__