#pragma once
#ifndef __WEAPON_SMG_H__
#define __WEAPON_SMG_H__

#include "PlayerWeapon.h"

enum class eSMGAction
{
	Reload, Idle, Fire4, Fire3, Fire2, Fire1, Draw, AltFire, END
};

class CWeaponSMG final : public CPlayerWeapon
{
private:
	explicit CWeaponSMG(_Device pDevice);
	explicit CWeaponSMG(const CWeaponSMG& other);
	virtual ~CWeaponSMG() = default;

public:
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
	virtual void Change_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:	
	static CWeaponSMG* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


};


#endif // !__WEAPON_SMG_H__