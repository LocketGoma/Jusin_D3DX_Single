#pragma once

#ifndef __PROJECTILE_FLECHETTE_H__
#define __PROJECTILE_FLECHETTE_H__

#include "BaseProjectile.h"
class CProjFlechette : public CBaseProjectile
{
private:
	explicit CProjFlechette(_Device pDevice);
	explicit CProjFlechette(const CProjFlechette& other);
	virtual ~CProjFlechette() = default;

public:
	// CBaseProjectile��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;	

private:
	HRESULT Add_Component();


public:
	static CProjFlechette* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg) override;

private:
	virtual void Free();

//������ (��밡����) �Լ���
// public:
//	const _uint Get_Damage();
//	void Set_Damage(_uint iDamage);
//	const _float Get_Splash_Radius();
//	void Set_Splash_Radius(_float fRadius);
// 
//	const _vec3 Get_StartPos();
//	void Set_StartPos(_vec3 vPos);
//	const _vec3 Get_Direction();
//	void Set_Direction(_vec3 vDir);
// 
//public:
//	virtual void Set_Position(_vec3 vPos);
//	virtual void Set_Size(_vec3 vSize);
//	virtual _vec3 Get_Position();
//	virtual _vec3 Get_Size();
//
//	//�⺻ ��ȣ�ۿ� ��
//public:
//	const _uint Get_Weight();		//��ü ����
//	void Set_Weight(_uint iWeight);
//
//	const eForceType Get_ObjectType();
//	void Set_ObjectType(eForceType eType);
//
//	//���� 0¥�� �߻� �� �Ҹ�Ÿ��
//	const _float Get_LifeTime();	//0 ���� : ������
//	void Set_LifeTime(_float fTime);
//
//	//�߻� �� �ӵ�
//	const _float Get_Speed();
//	void Set_Speed(_float fSpeed);


};


#endif // !__PROJECTILE_FLECHETTE_H__