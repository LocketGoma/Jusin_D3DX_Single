#pragma once

#ifndef __WEAPON_RPG_H__
#define __WEAPON_RPG_H__

#include "PlayerWeapon.h"

//Holster가 없었나??
enum class eRPGAction
{
	Reload, Idle, Fire, Draw, END
};

//특수무기 2
class CWeaponRPG final : public CPlayerWeapon
{
private:
	explicit CWeaponRPG(_Device pDevice);
	explicit CWeaponRPG(const CWeaponRPG& other);
	virtual ~CWeaponRPG() = default;

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
	virtual bool Reload_Weapon() override;
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;
	
private:
	HRESULT			Add_Component(void);

public:
	static CWeaponRPG* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;
};


#endif // !__WEAPON_RPG_H__