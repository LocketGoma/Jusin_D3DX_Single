#pragma once

#ifndef __BASE_OBJECT_H__
#define __BASE_OBJECT_H__

#include "GameObject.h"
BEGIN_NAMESPACE(Engine)
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
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
			void Set_Direction(_vec3 vDir);
			void Set_ObjectType(eForceType eType);			
	virtual _vec3 Get_Position();
	virtual _vec3 Get_Size();
			_vec3 Get_Direction();
			const eForceType Get_ObjectType();

//상호작용 2
public:
	Engine::CControlSupportUnit* Get_SupportUnit();

	virtual _bool Check_RayCollision() PURE;
	virtual _bool Check_RayCollision_By_CollisionSphere() PURE;
	const Engine::CTransform* Get_Transform();

	//무게 0짜리 발사 시 소멸타임
	const _float Get_LifeTime();	//0 이하 : 무제한
	void Set_LifeTime(_float fTime);

	//발사 시 속도
	const _float Get_Speed();
	void Set_Speed(_float fSpeed);

protected:
	virtual void Free(void);

	_uint m_iWeight;
	_float m_fLifeTime;
	_float m_fSpeed;			//비행속도 (or 발사 힘)
	eForceType m_eForceType;

	_vec3 m_vStartPos;
	_vec3 m_vDirection;

	_float m_fHitboxSize;		//=fRadius

	Engine::COLIDETYPE eType;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;

};


#endif // !__BASE_OBJECT_H__