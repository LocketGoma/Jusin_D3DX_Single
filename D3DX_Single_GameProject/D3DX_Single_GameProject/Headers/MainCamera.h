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

//�� ���
private:
	_float			m_fLoopAngle;		//ī�޶� ����ũ��
	_bool			m_bShaking;			//�̹� ����ũ���ΰ� �ƴѰ� (�ߺ��߻� ����)
	_vec3			m_vShake;			//����ŷ ����
	_uint			m_iShakePatton;		//����ũ ����

	_float			m_fLoopRecoilAngle;	//ī�޶� �ݵ�ó����
	_float			m_fLoopRecoilTimer;	//ī�޶� �ݵ�ó���� - �ð� ����
	_bool			m_bRecoilShaking;	//�ݵ� ó����
	_bool			m_bRecoilShake;		//�ݵ� ó����
	_vec3			m_vRecoilShake;		//�ݵ� ó�� ����
	_vec3			m_vRecoilShakeLoop;	//�ݵ� ó�� ����
	_vec3			m_vRecoilShakeAdd;	//�ݵ� ó�� ����
	_float			m_fStartRecoil;		//�ݵ� ó���� ���� ��
	HRESULT			Shake_Camera(const _float& fDeltaTime);
	HRESULT			Recoil_Weapon_Camera(const _float& fDeltaTime);
	void			Set_Recoil_Weapon_Power(_float fPower);

	//���콺 ���� ����
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

	_float m_fAxisXSpeed;		//X�� ����
	_float m_fAxisYSpeed;		//Y�� ����
	_bool m_bMouseLock;			//���콺 �� ���

	_float m_fRotate;

	_float m_fRecoilPower;		

};


#endif // !__MAIN_CAMERA_H__