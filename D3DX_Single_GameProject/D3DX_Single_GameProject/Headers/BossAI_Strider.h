#pragma once

#ifndef __AI_BOSS_STRIDER_H__
#define __AI_BOSS_STRIDER_H__

#include "BaseAI.h"

//���� : ��Ʈ���̴� AI
class CBossAI_Strider : public CBaseAI
{
private:
	explicit CBossAI_Strider(_Device pDevice);
	explicit CBossAI_Strider(const CBossAI_Strider& other);
	virtual ~CBossAI_Strider() = default;


public:
	// CBaseAI��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	// CBaseAI��(��) ���� ��ӵ�
	virtual HRESULT Do_Spawn(const _float& fDeltaTime) override;
	virtual HRESULT Do_Appear(const _float& fDeltaTime) override;
	virtual HRESULT Do_Idle(const _float& fDeltaTime) override;
	virtual HRESULT Do_Movement(const _float& fDeltaTime) override;
	virtual HRESULT Do_Attack(const _float& fDeltaTime) override;

public:
	static CBossAI_Strider* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


//protected:
//	CDynamicObject* m_pControlUnit;		//AI�� ������ Ÿ��
//	Engine::CGameObject* m_pTargetUnit;		//AI�� ����� Ÿ��
//
////�÷��̾� ��Ÿ��� �ϴ� 5.f �� ����ؼ� ����Ұ�.
//protected:
//	_bool m_bReady;			//����� ����
//	_bool m_bSpawn;			//�����ߴ°�
//	_bool m_bAppear;		//�����̴°�
//	_bool m_bTrack;			//�������ΰ�
//	_bool m_bAttack;		//�������ΰ�
//	_bool m_bDodge;			//ȸ�����ΰ�
//	_bool m_bDodgeLock;			//ȸ�����ΰ�
//
//	_float m_fRangeToTarget;	//Ÿ�ٱ����� �Ÿ�
//	_float m_fDodgeTime;		//�����ð� - �帣�°�
//	_float m_fInvinTime;		//�����ð�
//	_float m_fDodgeCoolTime;	//ȸ�� ��Ÿ��
//	_float m_fDodgeCountTime;	//ȸ�� ��Ÿ�� - �帣�°�
//
//	int m_iHPState;				//���� HP���� (���º񱳿�)
//
//	eBasePatton m_eBasePatton;	//���� �˻��


};


#endif // !__AI_BOSS_STRIDER_H__