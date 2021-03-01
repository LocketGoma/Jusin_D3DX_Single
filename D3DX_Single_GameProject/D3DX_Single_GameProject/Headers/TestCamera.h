#pragma once

#ifndef __TESTCAMERA_H__
#define __TESTCAMERA_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
	//����� ��ī�̹ڽ� ����⵵ �ɵ�
	class CTransform;
	class CCameraComponent;
END_NAMESPACE

class CTestCamera  : public Engine::CGameObject
{
private:
	explicit CTestCamera(_Device pDevice);
	virtual ~CTestCamera() = default;


public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int	Update_GameObject(const _float& fTimeDelta) override;
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta) override;
	virtual HRESULT Render_GameObject(void) override;

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fTimeDelta);
	void			Mouse_Movement();
	void			Picking_Mouse();

//���콺 ���� ����
public:
	void Set_MouseSpeedX(_float fAxisX);
	void Set_MouseSpeedY(_float fAxisY);
	_float Get_MouseSpeedX();
	_float Get_MouseSpeedY();
public:
	static CTestCamera* Create(_Device pDevice);	

private :
	virtual void Free(void) override;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CCameraComponent* m_pCameraCom = nullptr;

	_float m_fAxisXSpeed;		//X�� ����
	_float m_fAxisYSpeed;		//Y�� ����
	_bool m_bMouseLock;			//���콺 �� ���

	_float m_fRotate;

};


#endif // !__TESTCAMERA_H__