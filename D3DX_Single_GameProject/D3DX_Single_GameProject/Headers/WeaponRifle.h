#pragma once

#ifndef __WEAPON_RIFLE_H__
#define __WEAPON_RIFLE_H__

#include "PlayerWeapon.h"

enum class eRifleAction
{
	Shake, Reload, Idle, Holster, AltFire, Fire, Draw, END
};

class CWeaponRifle final : public CPlayerWeapon
{
private:
	explicit CWeaponRifle(_Device pDevice);
	explicit CWeaponRifle(const CWeaponRifle& other);
	virtual ~CWeaponRifle() = default;

public:
	// CPlayerWeapon을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Draw_Weapon() override;
	virtual _bool Shoot_Weapon() override;
	virtual void AltShoot_Weapon() override;
	virtual bool Reload_Weapon() override;
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;
	virtual void Change_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponRifle* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};


#endif // !__WEAPON_RIFLE_H__