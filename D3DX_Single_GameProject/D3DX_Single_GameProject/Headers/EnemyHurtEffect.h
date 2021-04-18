#pragma once


//�Ѿ� ���߽� ƽ ������ ����Ʈ
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
	// CBaseEffect��(��) ���� ��ӵ�
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


	//_float m_fLifetime;			//����Ʈ ����
	//_float m_fNowTime;			//���� �帥 �ð�

	_vec3 m_vStartSize;			//���� ������
	_vec3 m_vWeightSize;		//����ġ
	_float m_fWeight;			//����ġ	- ���ΰ�

	_uint m_iEffectTexture;		//��� �ؽ��� ������


};

#endif