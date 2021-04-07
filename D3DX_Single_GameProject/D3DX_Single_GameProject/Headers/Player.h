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

//컴포넌트 및 컴포넌트 관리용
private:
	Engine::COLIDETYPE eType;		//충돌 판정용
	CPlayerWeapon* m_pWeapon[(_uint)eWeaponType::WEAPON_END];
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	Engine::CSphereCollider* m_pColliderCom = nullptr;
	eWeaponType m_pWeaponType;
	
//기본 변수들
private:
	int m_iHP;
	_uint m_iFullHP;
	_float m_fHitboxSize;

//무기 컨트롤용
private:
	_float m_fWeaponTimer = 0.f;
	_float m_fMaxWeaponTimer = 0.2f;
	_bool m_bShootState;		//사격 여부
	_bool m_bWeaponState;		//사격 가능 여부

//플레이어 컨트롤용
private:
	_float m_fJumpPower;		//점프 힘
	_float m_fNowJumpPos;		//현재 점프 높이
	_float m_fStartPos;			//점프 시작 높이
	_float m_fGravition;		//중력값
	_float m_fJumpTime;			//점프 시간
	_bool m_bJump;				//점프중임?
	_bool m_bJumpStart;			//점프 시작함?



	_float m_fNowMoveSpeed;		//현재 이동속도
	_float m_fWalkSpeed;		//걷는 속도
	_float m_fRunSpeed;			//달리는 속도

public:
	static CPlayer* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr);

private:
	virtual void Free() override;

	//내부 변수
	_float m_fHeight = 5.5f;	//플레이어 키


};


#endif // !__MAIN_PLAYER_H__