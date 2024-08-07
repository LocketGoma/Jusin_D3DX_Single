#pragma once

#ifndef __PROJECTILE_SHOTGUN_PELLET_H__
#define __PROJECTILE_SHOTGUN_PELLET_H__

#include "BaseProjectile.h"

class CAmmoParticle;

//���� �縴

class CProjShotgunPellet : public CBaseProjectile
{
private:
	explicit CProjShotgunPellet(_Device pDevice);
	explicit CProjShotgunPellet(const CProjShotgunPellet& other);
	virtual ~CProjShotgunPellet() = default;

public:
	// CBaseProjectile��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT Add_Component();

public:
	static CProjShotgunPellet* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free();


private:
	Engine::CVTXRectTexture* m_pBufferCom[(_uint)eEffectAxis::END] = {};
	Engine::CTexture* m_pTextureCom = nullptr;

	CAmmoParticle* m_pParticle = nullptr;
};
#endif // !__PROJECTILE_SHOTGUN_PELLET_H__