#pragma once
#ifndef __WEAPON_PISTOL_H__
#define __WEAPON_PISTOL_H__

#include "PlayerWeapon.h"

class CEffectMuzzle;

enum class ePistolAction
{
	Reload, IdleEmpty, Idle, HolsterEmpty, Holster, FireEmpty, Fire3, Fire2, Fire1, Fire0, DrawEmpty, Draw, END
};

class CWeaponPistol final :  public CPlayerWeapon
{
private:
	explicit CWeaponPistol(_Device pDevice);
	explicit CWeaponPistol(const CWeaponPistol& other);
	virtual ~CWeaponPistol() = default;

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

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponPistol* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	ePistolAction eAction;


	//Test
	CEffectMuzzle* m_pEffect;
};


#endif // !__WEAPON_PISTOL_H__