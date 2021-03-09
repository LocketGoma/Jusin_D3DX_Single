#pragma once

#ifndef __TESTCAMERA_H__
#define __TESTCAMERA_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
	//여기다 스카이박스 끼얹기도 될듯
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
	virtual _int	Update_GameObject(const _float& fDeltaTime) override;
	virtual _int	LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fDeltaTime);
	void			Mouse_Movement();
	void			Picking_Mouse();

//마우스 감도 관련
public:
	void Set_MouseSpeedX(_float fAxisX);
	void Set_MouseSpeedY(_float fAxisY);
	_float Get_MouseSpeedX();
	_float Get_MouseSpeedY();
public:
	static CTestCamera* Create(_Device pDevice);	
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private :
	virtual void Free(void) override;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CCameraComponent* m_pCameraCom = nullptr;

	_float m_fAxisXSpeed;		//X축 감도
	_float m_fAxisYSpeed;		//Y축 감도
	_bool m_bMouseLock;			//마우스 락 언락

	_float m_fRotate;

};


#endif // !__TESTCAMERA_H__