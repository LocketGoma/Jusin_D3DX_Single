#pragma once

#ifndef __LIGHT_STAGEA_H__
#define __LIGHT_STAGEA_H__





#include "GameObject.h"
#include "Base.h"
#include "Light.h"

class CStageALight : public Engine::CGameObject
{
private:
	explicit CStageALight(_Device pDevice);
	explicit CStageALight(const CStageALight& other);
	virtual ~CStageALight() = default;

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;

	//¾È¾¸
public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

private:
	HRESULT			Add_Component(void);
	HRESULT			Setup_Light();

private:
	Engine::CLight* m_pLight = nullptr;

public:
	static CStageALight* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;
};


#endif // !__LIGHT_STAGEA_H__