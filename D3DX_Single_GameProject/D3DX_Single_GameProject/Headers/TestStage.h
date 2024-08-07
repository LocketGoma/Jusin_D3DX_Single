#pragma once
#ifndef __TEST_STAGE_H__
#define __TEST_STAGE_H__

#include "Scene.h"
#include "Base.h"

class CNaviMeshController;

class CTestStage : public Engine::CScene
{
private:
	explicit CTestStage(_Device pDevice);
	virtual ~CTestStage(void) = default;

public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fDeltaTime);
	virtual _int		LateUpdate_Scene(const _float& fDeltaTime);
	virtual void		Render_Scene(void);

private :
	HRESULT Ready_Resource(_Device& pDevice);

private:
	HRESULT Add_Player_Layer(const _tchar* pLayerTag);
	HRESULT Add_Test_Layer(const _tchar* pLayerTag);
	HRESULT Add_Object_Layer(const _tchar* pLayerTag);
	HRESULT Add_ColiderBox_Layer(const _tchar* pLayerTag);
	HRESULT Add_Camera_Layer(const _tchar* pLayerTag);
	HRESULT Add_Environment_Layer(const _tchar* pLayerTag);
	HRESULT Add_Enemy_Layer(const _tchar* pLayerTag);
	HRESULT Add_Weapon_Layer(const _tchar* pLayerTag);	

public :
	static CTestStage* Create(_Device pDevice);

private :
	void Change_Scene(ESceneType eType);

	virtual void Free() override;

	CNaviMeshController* m_pNaviController;
	//�׽�Ʈ��
	_matrix	m_matWorld, m_matView, m_matProj;



};


#endif // !__TEST_STAGE_H__