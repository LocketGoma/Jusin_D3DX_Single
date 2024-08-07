#pragma once

#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "GameObject.h"
BEGIN_NAMESPACE(Engine)
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
class CShader;
END_NAMESPACE

//스테틱 오브젝트, 다이나믹 오브젝트 부모
//중력건, 컨트롤 서포트 유닛 처리용

//중력건에 영향을 받는 애들

//NONE : 상호작용 불가 (일반 탄약 등)
//PUSH : 밀치기 가능 (거대 오브젝트 등)
//PULL : 당기기 가능 (수류탄, 소형/중간크기 오브젝트 등) ( = PUSH 포함)
enum class eForceType
{
	NONE, PUSH, PULL, END
};
//중력건 판정
enum class eForceState
{
	NONE, GRAP, PULL, END
};
class CBaseObject abstract: public Engine::CGameObject
{
protected:
	explicit CBaseObject(_Device pDevice);
	explicit CBaseObject(const CBaseObject& other);
	virtual ~CBaseObject() = default;


public:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

//상호작용
public:
	virtual void Set_Position(_vec3 vPos);
	virtual void Set_Size(_vec3 vSize);
	virtual void Set_Direction(_vec3 vDir);
			void Set_HitEnable();
			void Set_ObjectType(eForceType eType);		
			void Set_SpeedLockState(_bool bLock);
			void Set_Rotation(Engine::ROTATION eRotate, float fRadian);

	virtual _vec3 Get_Position();
	virtual _vec3 Get_Size();
			_vec3 Get_Direction();
			const _float Get_Radius();
			const eForceType Get_ObjectType();
			_bool Get_SpeedLockState();
			_uint Get_Damage();

//상호작용 2
public:
	Engine::CControlSupportUnit* Get_SupportUnit();

	virtual _bool Check_RayCollision() PURE;
	virtual _bool Check_RayCollision_By_CollisionSphere() PURE;
	const Engine::CTransform* Get_Transform();

	//무게 0짜리 발사 시 소멸타임
	const _float Get_LifeTime();	//0 이하 : 무제한
	void Set_LifeTime(_float fTime);
	void Add_LifeTime(_float fTime);

	//발사 시 속도
	const _float Get_Speed();
	virtual void Set_Speed(_float fSpeed);

	//중력 적용 관련
	void Set_GravitionPower(_float fGravition);
	void Set_ClearGSpeed(_float fClearHeight = 0.f);

	virtual HRESULT Interaction(Engine::CGameObject* pTarget) PURE;

	//충돌 반사각
	virtual _vec3 Get_Reflection(_vec3 vDir, _vec3 vPos) PURE;
	eForceState Get_ForceState();
	void Set_ForceState(eForceState eState);

protected :
	virtual HRESULT Setup_ConstantTable(LPD3DXEFFECT& pEffect, _bool bDissolve = false) PURE;

protected:
	virtual void Free(void);

	_float m_fWeight;
	_float m_fLifeTime;
	_float m_fSpeed;			//비행속도 (or 발사 힘)
	_float m_fGravitionSpeed;	//중력 영향을 받은 속도
	_float m_fGravitionPower;	//중력값	
	_bool m_bSpeedLock;			//속도값 변화 가능 여부

	_vec3 m_vStartPos;
	_vec3 m_vDirection;

	_float m_fHitboxSize;		//=fRadius

	//중력건 상호작용 관련
	_vec3 m_vImForceDirection;	//외부 힘 주어지는 방향
	_float m_fImForcePower;		//외부 힘
	eForceType m_eForceType;	//중력건 판정 여부
	eForceState m_eForceState;	//중력건 잡힌지 여부

	_uint m_iDamage;			//공격력 (충돌 포함)
	_bool m_bAttackHitEnable;	//공격이 유효한가 (한대 때리면 바로 유효 빼버림)



	Engine::COLIDETYPE eType;

	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CShader* m_pShaderCom = nullptr;			//쉐이더

	Engine::CManagement* m_pManagement = nullptr;
	

};


#endif // !__BASE_OBJECT_H__