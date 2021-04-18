#pragma once

//√—æÀ ∏Ì¡ﬂΩ√ ∆Ω ≈Õ¡ˆ¥¬ ¿Ã∆Â∆Æ
#ifndef __EFFECT_AMMO_PARTICLE_H__
#define __EFFECT_AMMO_PARTICLE_H__

#include "BaseEffect.h"
class CAmmoParticle :  public CBaseEffect
{
private:
	explicit CAmmoParticle(_Device pDevice);
	explicit CAmmoParticle(const CAmmoParticle& other);
	virtual ~CAmmoParticle() = default;


public:
	// CBaseEffect¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


private:
	virtual HRESULT Add_Component();
	virtual void Free();

public:
	static CAmmoParticle* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;




};


#endif // !__EFFECT_AMMO_PARTICLE_H__