#pragma once

#ifndef __WEAPON_PHYS_CANNON_H__
#define __WEAPON_PHYS_CANNON_H__

#include "PlayerWeapon.h"

/// <summary>
/// �ʿ��Ѱ�
/// 
/// 1. Ÿ���� �������
/// 2. Ÿ�ٰ� �Ÿ��� ��� �Ǵ���
/// 3. �׳� ���� / �߻��ϱ�
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

//Ư������ 1
class CWeaponPhysCannon final : public CPlayerWeapon
{
private:
	explicit CWeaponPhysCannon(_Device pDevice);
	explicit CWeaponPhysCannon(const CWeaponPhysCannon& other);
	virtual ~CWeaponPhysCannon() = default;

public:
	// CPlayerWeapon��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Draw_Weapon() override;
	virtual void Shoot_Weapon() override;
	virtual void AltShoot_Weapon() override;
	virtual bool Reload_Weapon() override;			//����
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


	//���� ���� ��ü�� �����ִ��� �߷°��� ���ʿ䰡 ����?
	//Engine::CGameObject* m_pTarget;
	CBaseObject* m_pTarget;
	CBaseObject* m_pLookTarget;

	ePhysAction m_eAction;
	_vec3 m_vDir;			//�ٶ󺸴� ���� = �߻��ϴ� ����

	_float m_fGrapGap;

	_bool m_bShootLock;
};


#endif // !__WEAPON_PHYS_CANNON_H__