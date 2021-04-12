#pragma once
#ifndef __MAIN_STAGE_B_H__
#define __MAIN_STAGE_B_H__

#include "Scene.h"
#include "Base.h"

class CNaviMeshController;

class CMainStageB : public Engine::CScene
{
private:
	explicit CMainStageB(_Device pDevice);
	virtual ~CMainStageB(void) = default;

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

private :
	HRESULT Ready_Resource(_Device& pDevice);

private:
	HRESULT Add_Player_Layer(const _tchar* pLayerTag);
	HRESULT Add_Object_Layer(const _tchar* pLayerTag);
	HRESULT Add_Environment_Layer(const _tchar* pLayerTag);
	HRESULT Add_Enemy_Layer(const _tchar* pLayerTag);
	HRESULT Add_Weapon_Layer(const _tchar* pLayerTag);	

public :
	static CMainStageB* Create(_Device pDevice);

private :
	virtual void Free() override;

	CNaviMeshController* m_pNaviController;


};


#endif // !__MAIN_STAGE_B_H__