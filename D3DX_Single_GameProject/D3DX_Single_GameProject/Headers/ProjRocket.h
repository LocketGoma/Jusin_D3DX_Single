#pragma once

#ifndef __PROJECTILE_ROCKET_H__
#define __PROJECTILE_ROCKET_H__

#include "BaseProjectile.h"

class CWeaponRPG;

class CProjRocket : public CBaseProjectile
{

public:
	// CBaseProjectile을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	
public:

	
	
private:
	CWeaponRPG* m_pParent;			//발사한 부모 (필요한가요..?)

	virtual CGameObject* Clone(void* pArg = nullptr) override;
};


#endif // !__PROJECTILE_ROCKET_H__