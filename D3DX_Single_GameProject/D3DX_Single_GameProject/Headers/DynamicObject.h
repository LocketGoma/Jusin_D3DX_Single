#pragma once

//���� ��. AI�� ������ ���̳��� ������Ʈ��
#ifndef __DYNAMIC_OBJECT_H__

#include "GameObject.h"

class CDynamicObject abstract : public Engine::CGameObject
{
protected:
	explicit CDynamicObject(_Device pDevice);
	explicit CDynamicObject(const CDynamicObject& other);
	virtual ~CDynamicObject() = default;


//�⺻ �׼ǵ�
public:
	virtual void Go_Stright() PURE;				//����
	virtual void Go_Side() PURE;				//���̵� �̵�
	virtual void Do_Run() PURE;					//�޸�
	virtual void Do_Walk() PURE;				//����
	virtual void Do_Rotate() PURE;				//ȸ��
	virtual void Do_Attack() PURE;				//����
	
	//virtual void Do_Tracking() PURE;			//�� ����

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;

	virtual void	Free(void);

};


#endif // !__DYNAMIC_OBJECT_H__