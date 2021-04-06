#pragma once

#ifndef __ENEMY_ANTLION_H__
#define __ENEMY_ANTLION_H__


#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

//DigDie : 바닥 대기 모션
//DigOut : 등장 모션
//Flip : 중력건 맞았을때 모션
//RagDoll : 사망모션
//Attack : 그냥 공격모션
//Run : 이동모션
//Idle : 대기모션
enum class eAntLionAction
{
	DigDie, DigOut, Flip, RagDoll, AttackB, AttackA, Run, Idle, END
};

enum class eAntLionPatton
{
	Idle,Dodge,PattonA,END
};

class CEnemyAntLion : public CDynamicObject
{
private:
	explicit CEnemyAntLion(_Device pDevice);
	explicit CEnemyAntLion(const CEnemyAntLion& other);
	virtual ~CEnemyAntLion() = default;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual void Go_Stright(_float fDeltaTime) override;
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Run(_float fDeltaTime) override;
	virtual void Do_Walk(_float fDeltaTime) override;
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Attack(_float fDeltaTime, _uint iPatton = 0) override;
	virtual void Do_Idle(_float fDeltaTime) override;
	virtual void Do_Spawn(_float fDeltaTime) override;
	virtual void Do_Dead(_float fDeltaTime) override;
	virtual _bool Do_Dodge(_float fDeltatime) override;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual void Set_Animation(_uint iIndex) override;
	virtual _bool End_Animation_State_Force() override;	
	virtual _uint Get_Patton() override;

private:
	HRESULT	Add_Component(void);
	virtual void Free() override;

public:
	static CEnemyAntLion* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	eAntLionAction m_eAction;
	eAntLionAction m_ePrevAction;			//직전에 실행된 애니메이션

	eAntLionPatton m_ePatton;

	_vec3 vOriPos;
};

#endif