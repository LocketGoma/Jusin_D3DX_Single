#pragma once

#ifndef __PROJECTILE_PULSE_AMMO_H__
#define __PROJECTILE_PULSE_AMMO_H__

#include "BaseProjectile.h"
class CProjPulseAmmo : public CBaseProjectile
{
private:
	explicit CProjPulseAmmo(_Device pDevice);
	explicit CProjPulseAmmo(const CProjPulseAmmo& other);
	virtual ~CProjPulseAmmo() = default;

public:
	// CBaseProjectile을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT Add_Component();

public:
	static CProjPulseAmmo* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free();

};


#endif // !__PROJECTILE_PULSE_AMMO_H__