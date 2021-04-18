#pragma once


//총알 명중시 틱 터지는 이펙트
#ifndef __EFFECT_ENEMYHURT_EFFECT_H__
#define __EFFECT_ENEMYHURT_EFFECT_H__

#include "BaseEffect.h"
class CEnemyHurtEffect : public CBaseEffect
{
private:
	explicit CEnemyHurtEffect(_Device pDevice);
	explicit CEnemyHurtEffect(const CEnemyHurtEffect& other);
	virtual ~CEnemyHurtEffect() = default;

public:
	// CBaseEffect을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	virtual HRESULT Add_Component();
	virtual void Free();

public:
	static CEnemyHurtEffect* Create(_Device pDevice);
	virtual Engine::CGameObject* Clone(void* pArg = nullptr) override;


	//_float m_fLifetime;			//이펙트 수명
	//_float m_fNowTime;			//현재 흐른 시간

	_vec3 m_vStartSize;			//시작 사이즈
	_vec3 m_vWeightSize;		//가중치
	_float m_fWeight;			//가중치	- 사인값

	_uint m_iEffectTexture;		//몇번 텍스쳐 쓸건지


};

#endif