#pragma once

#ifndef __EFFECT_EXPLOSION_H__
#define __EFFECT_EXPLOSION_H__

#include "BaseEffect.h"

//익스플로전!!
class CExplosionEffect :  public CBaseEffect
{
private:
	explicit CExplosionEffect(_Device pDevice);
	explicit CExplosionEffect(const CExplosionEffect& other);
	virtual ~CExplosionEffect() = default;

public:
	// CBaseEffect을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	

public:
	static CExplosionEffect* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual	HRESULT Add_Component();
	virtual void Free();



	_uint m_iEffectCount;
	_uint m_iMaxEffect;
};


#endif // !__EFFECT_EXPLOSION_H__