#pragma once

#ifndef __ENEMY_ANTLION_H__
#define __ENEMY_ANTLION_H__


#include "DynamicObject.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CCollider;
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
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	// CDynamicObject을(를) 통해 상속됨
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
	// CDynamicObject을(를) 통해 상속됨
	virtual void Set_Animation(_uint iIndex) override;

private:
	HRESULT	Add_Component(void);
	virtual void Free() override;

public:
	static CEnemyAntLion* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	Engine::CDynamicMesh* m_pMeshCom = nullptr;
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CCollider* m_pColliderCom = nullptr;


};

#endif