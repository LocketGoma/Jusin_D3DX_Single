#pragma once

#ifndef __BASE_AI_H__
#define __BASE_AI_H__

//���� AI

#include "Base.h"
#include "GameObject.h"
#include "DynamicObject.h"

//Nospawn : �̵���
//Spawn : ����
//Ready : �غ���� (�ƾ���. �ٸ� AI ��� �۵��Ƚ�Ŵ)
//Idle : ���� ��� �� ���
//Recog : �� ����
//Track : �� ����
//Attack : �� ����
//Dodge : ȸ��
//Dead : �ֱ�
//Disappear : ����� (�ֱ� ����)
enum class eAIStatus
{
	NOSPAWN,SPAWN, READY,IDLE, RECOG, TRACK, ATTACK, DODGE, DEAD,DISAPPEAR, END
};

class CBaseAI : public Engine::CGameObject
{
protected:
	explicit CBaseAI(_Device pDevice);
	explicit CBaseAI(const CBaseAI& other);
	virtual ~CBaseAI() = default;

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
// CGameObject��(��) ���� ��ӵ�... �ٵ� �Ƚ��.
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	eAIStatus Get_State();
	void Set_State(eAIStatus eState);

public:
	HRESULT Set_ControlUnit(CDynamicObject* pUnit);
	HRESULT Set_Target(Engine::CGameObject* pTarget);

	void Set_StartSpawn();

//���� �Ǵ� �Լ���
protected:
	virtual HRESULT Do_Spawn(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Appear(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Idle(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Movement(const _float& fDeltaTime) PURE;
	virtual HRESULT Do_Attack(const _float& fDeltaTime) PURE;

	_bool Check_HP_Change();

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free() override;

protected:
			CDynamicObject*	m_pControlUnit;		//AI�� ������ Ÿ��
	Engine::CGameObject*	m_pTargetUnit;		//AI�� ����� Ÿ��

//�÷��̾� ��Ÿ��� �ϴ� 5.f �� ����ؼ� ����Ұ�.
protected:
	_bool m_bStartSpawn;	//���� Ʈ���� (�̰� �������� �ƹ��͵� ����) <- Ready ��� �Ƴ�?
	_bool m_bReady;			//����� ����
	_bool m_bSpawn;			//�����ߴ°�
	_bool m_bAppear;		//�����̴°�
	_bool m_bTrack;			//�������ΰ�
	_bool m_bAttack;		//�������ΰ�
	_bool m_bDodge;			//ȸ�����ΰ�
	_bool m_bDodgeLock;			//ȸ�����ΰ�

	_float m_fRangeToTarget;	//Ÿ�ٱ����� �Ÿ�
	_float m_fDodgeTime;		//�����ð� - �帣�°�
	_float m_fInvinTime;		//�����ð�
	_float m_fDodgeCoolTime;	//ȸ�� ��Ÿ��
	_float m_fDodgeCountTime;	//ȸ�� ��Ÿ�� - �帣�°�

	int m_iHPState;				//���� HP���� (���º񱳿�)

	eBasePatton m_eBasePatton;	//���� �˻��
	eAIStatus	m_eState;		//AI ���� ���� �˻� (������ � ���)

};

#endif