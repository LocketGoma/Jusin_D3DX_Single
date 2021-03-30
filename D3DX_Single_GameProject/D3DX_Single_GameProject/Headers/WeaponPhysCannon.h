#pragma once

#ifndef __WEAPON_PHYS_CANNON_H__
#define __WEAPON_PHYS_CANNON_H__

#include "PlayerWeapon.h"

enum class ePhysAction
{
	Idle, Holster, Hold_Idle, Fire, Draw, AltFire, END
};

//특수무기 1
class CWeaponPhysCannon final : public CPlayerWeapon
{
private:
	explicit CWeaponPhysCannon(_Device pDevice);
	explicit CWeaponPhysCannon(const CWeaponPhysCannon& other);
	virtual ~CWeaponPhysCannon() = default;

public:
	// CPlayerWeapon을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Draw_Weapon() override;
	virtual void Shoot_Weapon() override;
	virtual void AltShoot_Weapon() override;
	virtual void Reload_Weapon() override;			//없음
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponPhysCannon* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

};


#endif // !__WEAPON_PHYS_CANNON_H__