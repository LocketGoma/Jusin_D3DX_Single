#pragma once

#ifndef __MAIN_PLAYER_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
class CControlSupportUnit;

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

private:
	HRESULT			Add_Component(void);
	void			Key_Input(const _float& fDeltaTime);


private:
	CPlayerWeapon* m_pWeapon[(_uint)eWeaponType::WEAPON_END];
	Engine::CTransform* m_pTransformCom = nullptr;
	Engine::CControlSupportUnit* m_pSupportCom = nullptr;
	eWeaponType m_pWeaponType;

//무기 컨트롤용
private:
	_float m_fWeaponTimer = 0.f;
	_float m_fMaxWeaponTimer = 0.2f;
	_bool m_bShootState;

public:
	static CPlayer* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr);

private:
	virtual void Free() override;


};


#endif // !__MAIN_PLAYER_H__