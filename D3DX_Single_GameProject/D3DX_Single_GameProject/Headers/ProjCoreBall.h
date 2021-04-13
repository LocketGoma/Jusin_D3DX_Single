#pragma once

//펄스 라이플 2차 발사
#ifndef __PROJECTILE_COREBALL_H__
#define __PROJECTILE_COREBALL_H__

#include "BaseProjectile.h"
class CProjCoreBall :  public CBaseProjectile
{
private:
	explicit CProjCoreBall(_Device pDevice);
	explicit CProjCoreBall(const CProjCoreBall& other);
	virtual ~CProjCoreBall() = default;

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
	static CProjCoreBall* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg) override;

private:
	virtual void Free();

	Engine::CTexture* m_pTexture;
	LPD3DXMESH m_pMesh;

	//숨겨진 (사용가능한) 함수들
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
	//	//기본 상호작용 값
	//public:
	//	const _uint Get_Weight();		//물체 무게
	//	void Set_Weight(_uint iWeight);
	//
	//	const eForceType Get_ObjectType();
	//	void Set_ObjectType(eForceType eType);
	//
	//	//무게 0짜리 발사 시 소멸타임
	//	const _float Get_LifeTime();	//0 이하 : 무제한
	//	void Set_LifeTime(_float fTime);
	//
	//	//발사 시 속도
	//	const _float Get_Speed();
	//	void Set_Speed(_float fSpeed);

};

#endif // !__PROJECTILE_COREBALL_H__