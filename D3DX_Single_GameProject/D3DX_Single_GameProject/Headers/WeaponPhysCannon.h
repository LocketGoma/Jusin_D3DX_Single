#pragma once

#ifndef __WEAPON_PHYS_CANNON_H__
#define __WEAPON_PHYS_CANNON_H__

#include "PlayerWeapon.h"

/// <summary>
/// 필요한것
/// 
/// 1. 타겟이 어떤것인지
/// 2. 타겟간 거리는 어떻게 되는지
/// 3. 그냥 놓기 / 발사하기
/// </summary>
/// 

class CBaseObject;

enum class ePhysTargetType
{
	Static, Dynamic, None, END
};

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
	virtual bool Reload_Weapon() override;			//없음
	virtual void Release_Weapon() override;
	virtual void Holster_Weapon() override;
	virtual void Change_Weapon() override;

private:
	HRESULT			Add_Component(void);

public:
	static CWeaponPhysCannon* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

	void Set_TargetObject(CBaseObject* pTarget);
	CBaseObject* Get_TargetObject();
	CBaseObject* Get_LookTargetObject();

private:
	virtual void Free() override;


	//내가 무슨 물체를 갖고있는지 중력건이 알필요가 없죠?
	//Engine::CGameObject* m_pTarget;
	CBaseObject* m_pTarget;
	CBaseObject* m_pLookTarget;

	ePhysAction m_eAction;
	_vec3 m_vDir;			//바라보는 방향 = 발사하는 방향

	_float m_fGrapGap;

	_bool m_bShootLock;
};


#endif // !__WEAPON_PHYS_CANNON_H__