#pragma once

#ifndef __WEAPON_SHOTGUN_H__
#define __WEAPON_SHOTGUN_H__

#include "PlayerWeapon.h"

enum class eShotgunAction
{
	Reload3, Reload2, Reload1, Reload, Pump, Idle, Holster, Fire, DryFire, Draw, AltFire, END
};

class CWeaponShotgun final : public CPlayerWeapon
{
private:
	explicit CWeaponShotgun(_Device pDevice);
	explicit CWeaponShotgun(const CWeaponShotgun& other);
	virtual ~CWeaponShotgun() = default;


	// CPlayerWeapon을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	// CPlayerWeapon을(를) 통해 상속됨
	virtual void Draw_Weapon() override;
	virtual void Shoot_Weapon() override;
	virtual void AltShoot_Weapon() override;
	virtual bool Reload_Weapon() override;
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponShotgun* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

private:
	_bool	m_bReloading;

};

#endif // !__WEAPON_SHOTGUN_H__
