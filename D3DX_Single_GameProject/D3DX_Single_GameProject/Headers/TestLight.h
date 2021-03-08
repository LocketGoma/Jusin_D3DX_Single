#pragma once

#ifndef __LIGHT_TEST_H__
#define __LIGHT_TEST_H__

#include "GameObject.h"
#include "Base.h"
#include "Light.h"

class CTestLight : public Engine::CGameObject
{
private:
	explicit CTestLight(_Device pDevice);
	virtual ~CTestLight() = default;

public:
	virtual HRESULT Ready_GameObject() override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;


private:
	HRESULT			Add_Component(void);
	HRESULT			Setup_Light();

private:
	Engine::CLight* m_pLight = nullptr;

public:
	static CTestLight* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;



};


#endif // !__LIGHT_TEST_H__