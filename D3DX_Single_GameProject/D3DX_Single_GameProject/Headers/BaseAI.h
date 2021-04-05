#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//���� AI

#include "Base.h"
#include "GameObject.h"
#include "DynamicObject.h"

class CBaseAI : public Engine::CGameObject
{
private:
	explicit CBaseAI(_Device pDevice);
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI() = default;

public:
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
// CGameObject��(��) ���� ��ӵ�... �ٵ� �Ƚ��.
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	HRESULT Set_ControlUnit(CDynamicObject* pUnit);
	HRESULT Set_Target(Engine::CGameObject* pTarget);

//���� �Ǵ� �Լ���
private:
	HRESULT Do_Spawn(const _float& fDeltaTime);
	HRESULT Do_Appear(const _float& fDeltaTime);
	HRESULT Do_Idle(const _float& fDeltaTime);
	HRESULT Do_Movement(const _float& fDeltaTime);
	HRESULT Do_Attack(const _float& fDeltaTime);

	_bool Check_HP_Change();

public:
	static CBaseAI* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

	virtual void Free() override;

private:
			CDynamicObject*	m_pControlUnit;		//AI�� ������ Ÿ��
	Engine::CGameObject*	m_pTargetUnit;		//AI�� ����� Ÿ��

//�÷��̾� ��Ÿ��� �ϴ� 5.f �� ����ؼ� ����Ұ�.
private:
	_bool m_bReady;			//����� ����
	_bool m_bSpawn;			//�����ߴ°�
	_bool m_bAppear;		//�����̴°�
	_bool m_bTrack;			//�������ΰ�
	_bool m_bAttack;		//�������ΰ�
	_bool m_bDodge;			//ȸ�����ΰ�

	_float m_fRangeToTarget;	//Ÿ�ٱ����� �Ÿ�
	_float m_fDodgeTime;		//�����ð� - �帣�°�
	_float m_fInvinTime;		//�����ð�

	int m_iHPState;				//���� HP���� (���º񱳿�)


};

#endif