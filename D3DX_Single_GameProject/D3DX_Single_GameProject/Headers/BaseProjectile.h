#pragma once

#ifndef __BASE_PROJECTILE_H__
#define __BASE_PROJECTILE_H__

#include "StaticObject.h"
#include "GameObject.h"
#include "Base.h"

//ToPlayer : �÷��̾�� ����� ����
//ToEnemy : ������ ����� ����
//Both : �Ѵ� ����� ���� (�ǽð� ����� �ƴ�)
enum class eTargetState
{
	ToPlayer, ToEnemy, Both, None, END
};

class CBaseProjectile abstract : public CStaticObject
{
protected:
	explicit CBaseProjectile(_Device pDevice);
	explicit CBaseProjectile(const CBaseProjectile& other);
	virtual ~CBaseProjectile() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
	const _uint Get_Damage();
	void Set_Damage(_uint iDamage);
	const _float Get_Radius();
	void Set_Radius(_float fRadius);
	const _float Get_Splash_Radius();
	void Set_Splash_Radius(_float fRadius);

	const _vec3 Get_StartPos();
	void Set_StartPos(_vec3 vPos);
	const _vec3 Get_Direction();
	void Set_Direction(_vec3 vDir);

	eTargetState Get_TargetState();
	void Set_TargetState(eTargetState eState);

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);


protected:
	_uint m_iDamage;
	_float m_fSplashRadius;
	_float m_fRotate;			//���ۺ��� (�־�̰� �ϴ� �뵵��)

	_vec3 m_vAmmoSize;			//������ ���̴� ũ��

	eTargetState m_eTargetState;

};


#endif // !__BASE_PROJECTILE_H__