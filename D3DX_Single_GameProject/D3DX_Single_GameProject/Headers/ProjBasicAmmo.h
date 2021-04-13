#pragma once

#ifndef __PROJECTILE_BASIC_AMMO_H__
#define __PROJECTILE_BASIC_AMMO_H__

#include "BaseProjectile.h"

//SMG, ±«√—
class CProjBasicAmmo : public CBaseProjectile
{
private:
	explicit CProjBasicAmmo(_Device pDevice);
	explicit CProjBasicAmmo(const CProjBasicAmmo& other);
	virtual ~CProjBasicAmmo() = default;

public:
	// CBaseProjectile¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT Add_Component();

public:
	static CProjBasicAmmo* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free();


private:
	Engine::CVTXRectTexture* m_pBufferCom[(_uint)eEffectAxis::END] = {};
	Engine::CTexture* m_pTextureCom = nullptr;


};



#endif