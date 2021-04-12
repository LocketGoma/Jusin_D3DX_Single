#pragma once

#ifndef __LIGHT_STAGEC_H__
#define __LIGHT_STAGEC_H__

#include "GameObject.h"
#include "Base.h"
#include "Light.h"

class CStageCLight : public Engine::CGameObject
{
private:
	explicit CStageCLight(_Device pDevice);
	explicit CStageCLight(const CStageCLight& other);
	virtual ~CStageCLight() = default;


public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject() override;



//æ»æ∏
public:
	// CGameObject¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
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
	static CStageCLight* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};


#endif // !__LIGHT_STAGEC_H__