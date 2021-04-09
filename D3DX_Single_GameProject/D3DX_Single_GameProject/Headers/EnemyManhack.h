#pragma once

#ifndef __ENEMY_MANHACK_H__
#define __ENEMY_MANHACK_H__


#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE
//중력건으로 잡을 수 있음!
//= 외부에서 속도, 공격상태를 조절하는 함수가 들어가야됨.

//+ 중력 영향 안받음

enum class eManhackAction
{
	Netual, Idle, Fly1, Fly2, Fly3, Deploy, Dead, END
};
enum class eManhackPatton
{
	Idle,Dodge,PattonA,END
};

class CEnemyManhack : public CDynamicObject
{
private:
	explicit CEnemyManhack(_Device pDevice);
	explicit CEnemyManhack(const CEnemyManhack& other);
	virtual ~CEnemyManhack() = default;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float & fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float & fDeltaTime) override;
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
	static CEnemyManhack* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	eManhackAction m_eAction;
	eManhackAction m_ePrevAction;

	eManhackPatton m_ePatton;

	_vec3 m_vOriPos;

	_float m_fVerticalRotate;

	eAlign m_ePreHoriAlign;

};


#endif // !__ENEMY_MANHACK_H__