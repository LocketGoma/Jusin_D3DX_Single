#pragma once

#ifndef __PROJECTILE_SHOTGUN_SHELL_H__
#define __PROJECTILE_SHOTGUN_SHELL_H__

//º¶∞« Ω©

#include "BaseProjectile.h"
class CProjShotgunShell :  public CBaseProjectile
{
private:
	explicit CProjShotgunShell(_Device pDevice);
	explicit CProjShotgunShell(const CProjShotgunShell& other);
	virtual ~CProjShotgunShell() = default;

public:
	// CBaseProjectile¿ª(∏¶) ≈Î«ÿ ªÛº”µ 
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

	void Set_ShellNumber(_uint iShellNumber);

private:
	HRESULT Shoot_Pellet(_uint iPelletCount);				//∆Á∏¥ πﬂªÁ!

	HRESULT Add_Component();

public:
	static CProjShotgunShell* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free();

	_float m_fAxisDifferenceRange_X;
	_float m_fAxisDifferenceRange_Y;

	_uint m_iPelletCount;
	_uint m_iShellNumber;

};


#endif // !__PROJECTILE_SHOTGUN_SHELL_H__