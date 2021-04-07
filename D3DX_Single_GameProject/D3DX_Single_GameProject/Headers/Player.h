#pragma once

#ifndef __MAIN_PLAYER_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;
class CSphereCollider;
END_NAMESPACE

class CPlayerWeapon;

enum class eWeaponType
{
	WEAPON_CROWBAR, WEAPON_PISTOL, WEAPON_SMG, WEAPON_RIFLE, WEAPON_SHOTGUN, WEAPON_PHYCANNON, WEAPON_RPG, WEAPON_END
};

class CPlayer : public Engine::CGameObject
{
private:
	explicit CPlayer(_Device pDevice);
	explicit CPlayer(const CPlayer& other);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Ready_GameObject(_uint iTexNumber = 0);
	virtual HRESULT Ready_GameObject_Clone(void* pArg) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

	static void MouseProc(UINT message, WPARAM wParam);
	static float g_zDelta;

public:
	void Set_Animation(int iNumber);
	Engine::CAnimationController* Get_AniController();
	int Get_VertexNumber();

	_uint Get_WeaponDamage();

	bool Hit_Attack(_uint iDamage);

private:
	HRESULT			Add_Component(void);
	HRESULT			Print_TestUI();
	void			Key_Input(const _float& fDeltaTime);
	_bool			Jump_Action(const _float& fDeltaTime);
	

public:
	_bool			Check_Attack_Collide(const _vec3* pSourcePos, const _float fSourceRadius);
	void			Jump_Cancel();

//������Ʈ �� ������Ʈ ������
private:
	Engine::COLIDETYPE eType;		//�浹 ������
	CPlayerWeapon* m_pWeapon[(_uint)eWeaponType::WEAPON_END];
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;
	eWeaponType m_pWeaponType;
	
//�⺻ ������
private:
	int m_iHP;
	_uint m_iFullHP;
	_float m_fHitboxSize;

//���� ��Ʈ�ѿ�
private:
	_float m_fWeaponTimer = 0.f;
	_float m_fMaxWeaponTimer = 0.2f;
	_bool m_bShootState;		//��� ����
	_bool m_bWeaponState;		//��� ���� ����

//�÷��̾� ��Ʈ�ѿ�
private:
	_float m_fJumpPower;		//���� ��
	_float m_fNowJumpPos;		//���� ���� ����
	_float m_fStartPos;			//���� ���� ����
	_float m_fGravition;		//�߷°�
	_float m_fJumpTime;			//���� �ð�
	_bool m_bJump;				//��������?
	_bool m_bJumpStart;			//���� ������?



	_float m_fNowMoveSpeed;		//���� �̵��ӵ�
	_float m_fWalkSpeed;		//�ȴ� �ӵ�
	_float m_fRunSpeed;			//�޸��� �ӵ�

public:
	static CPlayer* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr);

private:
	virtual void Free() override;

	//���� ����
	_float m_fHeight = 5.5f;	//�÷��̾� Ű


};


#endif // !__MAIN_PLAYER_H__