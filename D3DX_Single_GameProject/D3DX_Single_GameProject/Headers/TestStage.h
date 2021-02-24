#pragma once
#ifndef __TEST_STAGE_H__
#define __TEST_STAGE_H__

#include "Scene.h"
#include "Base.h"

class CTestStage : public Engine::CScene
{
private:
	explicit CTestStage(_Device pDevice);
	virtual ~CTestStage(void) = default;

public:
public:
	virtual HRESULT		Ready_Scene(void);
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual _int		LateUpdate_Scene(const _float& fTimeDelta);
	virtual void		Render_Scene(void);

private :
	HRESULT Ready_Resource(_Device& pDevice);

private:
	HRESULT Add_Player_Layer(const _tchar* pLayerTag);

public :
	static CTestStage* Create(_Device pDevice);

private :
	virtual void Free() override;

	//테스트용
	_matrix	m_matWorld, m_matView, m_matProj;

};


#endif // !__TEST_STAGE_H__