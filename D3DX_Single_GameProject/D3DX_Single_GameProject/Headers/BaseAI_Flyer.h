#pragma once

#ifndef __AI_BASE_FLYER_H__
#define __AI_BASE_FLYER_H__

#include "BaseAI.h"

//����, �Ʒ���, ����
enum class eAlignHeight
{
	toUp, toDown, Horizontal, END
};

//3���� �̵��� �ʿ��� ��ü��
class CBaseAI_Flyer : public CBaseAI
{
private:
	explicit CBaseAI_Flyer(_Device pDevice);
	explicit CBaseAI_Flyer(const CBaseAI_Flyer& other);
	virtual ~CBaseAI_Flyer() = default;

public:
	// CBaseAI��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject() override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float & fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float & fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;


public:
	// CBaseAI��(��) ���� ��ӵ�
	virtual HRESULT Do_Spawn(const _float & fDeltaTime) override;
	virtual HRESULT Do_Appear(const _float & fDeltaTime) override;
	virtual HRESULT Do_Idle(const _float & fDeltaTime) override;
	virtual HRESULT Do_Movement(const _float & fDeltaTime) override;
	virtual HRESULT Do_Attack(const _float & fDeltaTime) override;

public:
	static CBaseAI_Flyer* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	eAlignHeight m_eHeightState;

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

#endif