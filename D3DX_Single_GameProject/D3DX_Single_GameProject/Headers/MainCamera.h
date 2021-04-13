#pragma once

#ifndef __MAIN_CAMERA_H__
#define __MAIN_CAMERA_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CTransform;
class CCameraComponent;
END_NAMESPACE
class CPlayer;

class CMainCamera : public Engine::CGameObject
{
private:
	explicit CMainCamera(_Device pDevice);
	explicit CMainCamera(const CMainCamera& other);
	virtual ~CMainCamera() = default;

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int	Update_GameObject(const _float& fDeltaTime) override;
	virtual _int	LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;
	HRESULT Set_Player(CPlayer* pPlayer);

private:
	HRESULT			Add_Component(void);
	HRESULT			Base_Movement(const _float& fDeltatime);
	void			Key_Input(const _float& fDeltaTime);
	void			Mouse_Movement();
	void			Picking_Mouse();

//잡 기능
private:
	_float			m_fLoopAngle;		//카메라 쉐이크용
	_bool			m_bShaking;			//이미 쉐이크중인가 아닌가 (중복발생 방지)
	_vec3			m_vShake;			//쉐이킹 벡터
	_uint			m_iShakePatton;		//쉐이크 패턴
	HRESULT			Shake_Camera(const _float& fDeltaTime);

	//마우스 감도 관련
public:
	void Set_MouseSpeedX(_float fAxisX);
	void Set_MouseSpeedY(_float fAxisY);
	_float Get_MouseSpeedX();
	_float Get_MouseSpeedY();

public :
	static CMainCamera* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free() override;

	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CCameraComponent* m_pCameraCom = nullptr;
	CPlayer* m_pPlayer = nullptr;

	_float m_fAxisXSpeed;		//X축 감도
	_float m_fAxisYSpeed;		//Y축 감도
	_bool m_bMouseLock;			//마우스 락 언락

	_float m_fRotate;

};


#endif // !__MAIN_CAMERA_H__