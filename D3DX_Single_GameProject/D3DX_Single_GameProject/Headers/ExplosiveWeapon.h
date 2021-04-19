#pragma once

//드럼통 터지는것 등
#ifndef __EXPLOSIVE_WEAPON_H__
#define __EXPLOSIVE_WEAPON_H__

#include "BaseProjectile.h"
class CExplosiveWeapon :  public CBaseProjectile
{
private:
	explicit CExplosiveWeapon(_Device pDevice);
	explicit CExplosiveWeapon(const CExplosiveWeapon& other);
	virtual ~CExplosiveWeapon() = default;

public:
	// CBaseProjectile을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT Add_Component();

public:
	static CExplosiveWeapon* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg) override;

private:
	virtual void Free();


};


#endif // !__EXPLOSIVE_WEAPON_H__