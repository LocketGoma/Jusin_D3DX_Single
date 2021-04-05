#pragma once
#ifndef __ENEMY_STRIDER_BOSS_H__
#define __ENEMY_STRIDER_BOSS_H__

#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

enum class eStriderAction
{
	Surprise_wait, Surprise, Stomp, Stand, Sleep, RagDoll, idle_low, idle_high, hitR, fastwalkW, fastwalkS, fastwalkN, fastwalkE, Crouch, END
};

enum class eStriderPatton
{
	Idle, END
};

class CBossStrider : public CDynamicObject
{
private:
	explicit CBossStrider(_Device pDevice);
	explicit CBossStrider(const CBossStrider& other);
	virtual ~CBossStrider() = default;

public:
	// CDynamicObject을(를) 통해 상속됨
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	virtual void Go_Stright(_float fDeltaTime) override;
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Run(_float fDeltaTime) override;
	virtual void Do_Walk(_float fDeltaTime) override;
	virtual void Do_Rotate(_float fDeltaTime, eAlign pAlign) override;
	virtual void Do_Attack(_float fDeltaTime) override;
	virtual void Do_Idle(_float fDeltaTime) override;
	virtual void Do_Spawn(_float fDeltaTime) override;
	virtual void Do_Dead(_float fDeltaTime) override;

public:
	virtual void Set_Animation(_uint iIndex) override;
	virtual _bool End_Animation_State_Force() override;


private:
	void PattonA();
	void PattonB();
	void PattonC();


private:
	HRESULT Add_Component();
	virtual void Free() override;

public:
	static CBossStrider* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	eStriderAction eAction;
	eStriderAction ePrevAction;
	eStriderPatton ePatton;


	_vec3 vOriPos;
	_vec3 vCalibrationPos;		//메쉬 좌표 뒤틀림으로 인한 위치 보정

};


#endif // !__ENEMY_STRIDER_BOSS_H__