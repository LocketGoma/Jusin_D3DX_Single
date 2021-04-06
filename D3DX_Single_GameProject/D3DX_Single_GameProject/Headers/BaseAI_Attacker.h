#pragma once

#ifndef __AI_BASE_ATTACKER_H__
#define __AI_BASE_ATTACKER_H__

#include "BaseAI.h"

//���ũ��, ���̱ͽ� �� �ܼ� �������ݰ�
class CBaseAI_Attacker : public CBaseAI
{
private:
	explicit CBaseAI_Attacker(_Device pDevice);
	explicit CBaseAI_Attacker(const CBaseAI_Attacker& other);
	virtual ~CBaseAI_Attacker() = default;

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
	static CBaseAI_Attacker* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;


	//_bool m_bReady;			//����� ����
	//_bool m_bSpawn;			//�����ߴ°�
	//_bool m_bAppear;		//�����̴°�
	//_bool m_bTrack;			//�������ΰ�
	//_bool m_bAttack;		//�������ΰ�
	//_bool m_bDodge;			//ȸ�����ΰ�

	//_float m_fRangeToTarget;	//Ÿ�ٱ����� �Ÿ�
	//_float m_fDodgeTime;		//�����ð� - �帣�°�
	//_float m_fInvinTime;		//�����ð�

	//int m_iHPState;				//���� HP���� (���º񱳿�)

};

#endif // !__AI_BASE_ATTACKER_H__
