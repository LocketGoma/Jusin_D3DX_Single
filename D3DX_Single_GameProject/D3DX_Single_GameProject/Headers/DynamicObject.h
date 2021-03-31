#pragma once

//���� ��. AI�� ������ ���̳��� ������Ʈ��
#ifndef __DYNAMIC_OBJECT_H__

#include "GameObject.h"

enum class eAlign
{
	LEFT, RIGHT, END
};

class CDynamicObject abstract : public Engine::CGameObject
{
protected:
	explicit CDynamicObject(_Device pDevice);
	explicit CDynamicObject(const CDynamicObject& other);
	virtual ~CDynamicObject() = default;

public:
	// CGameObject��(��) ���� ��ӵ�
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

public:
	virtual void Set_Position(_vec3 vPos) PURE;
	virtual void Set_Size(_vec3 vSize) PURE;
	virtual _vec3 Get_Position() PURE;
	virtual _vec3 Get_Size() PURE;

//�⺻ �׼ǵ� - AI���� ����
public:
	virtual void Go_Stright(_float fDeltaTime) PURE;				//����
	virtual void Go_Side(_float fDeltaTime, eAlign pAlign) PURE;	//���̵� �̵�
	virtual void Do_Run(_float fDeltaTime) PURE;				//�޸�
	virtual void Do_Walk(_float fDeltaTime) PURE;				//����
	virtual void Do_Rotate(_float fDeltaTime) PURE;				//ȸ��
	virtual void Do_Attack(_float fDeltaTime) PURE;				//����
	virtual void Do_Idle(_float fDeltaTime) PURE;				//��� + ����
	virtual void Do_Spawn(_float fDeltaTime) PURE;				//���� (���� ����)
	virtual void Do_Dead(_float fDeltaTime) PURE;				//���

	//virtual void Do_Tracking() PURE;			//�� ����

//�ܺ� �ִϸ��̼� ���ۿ�
public:
	virtual void Set_Animation(_uint iIndex) PURE;

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void Free(void);

protected:
	//������
	_float m_fMoveSpeed;
	_float m_fAnimationSpeed;

	//�⺻ ���� ����
	_float m_fAttackInterval;
};


#endif // !__DYNAMIC_OBJECT_H__