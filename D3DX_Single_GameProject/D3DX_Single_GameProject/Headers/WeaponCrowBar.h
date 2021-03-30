#pragma once

#ifndef __WEAPON_CROWBAR_H__
#define __WEAPON_CROWBAR_H__

#include "PlayerWeapon.h"

enum class eCrowbarAction
{
	MissCenter2, MissCenter1, Idle, Holster, HitKill, HitCenter3, HitCenter2, HitCenter1, Draw, END
};

class CWeaponCrowbar final : public CPlayerWeapon
{
private:
	explicit CWeaponCrowbar(_Device pDevice);
	explicit CWeaponCrowbar(const CWeaponCrowbar& other);
	virtual ~CWeaponCrowbar() = default;

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
	virtual void Reload_Weapon() override;
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponCrowbar* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;



private:
	virtual void Free() override;

private:
	eCrowbarAction	eAction;
	_bool			m_bZoom;

};


#endif // !__WEAPON_CROWBAR_H__