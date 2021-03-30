#pragma once
#ifndef __PLAYER_WEAPON_H__
#define __PLAYER_WEAPON_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
class CTransform;
END_NAMESPACE

class CPlayerWeapon abstract : public Engine::CGameObject
{
protected:
	explicit CPlayerWeapon(_Device pDevice);
	explicit CPlayerWeapon(const CPlayerWeapon& other);
	virtual ~CPlayerWeapon() = default;

public:
	virtual HRESULT Ready_GameObject() PURE;
	virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
	virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
	virtual HRESULT Render_GameObject(void) PURE;

//공격/장전
public:
	virtual void Draw_Weapon() PURE;		//무기 꺼내기
	virtual void Shoot_Weapon() PURE;		//1차사격
	virtual void AltShoot_Weapon() PURE;	//2차사격
	virtual void Reload_Weapon();			//재장전
	virtual void Release_Weapon() PURE;		//사격종료
	virtual void Holster_Weapon() PURE;		//무기 집어넣기

//탄 Get/Set
public:
	_uint Get_RemainAmmo();				//현재 남은 탄환
	_uint Get_MagAmmo();				//탄창에 남은 탄환
	_uint Get_RemainAltAmmo();			//현재 남은 2차 탄환
	void Add_Ammo(_uint _iAmmo);		//탄환 추가
	void Add_AltAmmo(_uint _iAmmo);		//2차 탄환 추가

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	void Set_Animation(int iNumber);
	int Get_VertexNumber();

protected:
	Engine::CDynamicMesh* m_pMeshCom;
	Engine::CTransform* m_pTransformCom = nullptr;
	_uint m_iMagAmmo;			//탄창 남은 탄환
	_uint m_iMainAmmo;			//소지중 탄환
	_uint m_iAltAmmo;			//소지중 2차탄환
	_uint m_iMaxAmmo;			//최대 소지가능 탄환
	_uint m_iMaxMagAmmo;		//탄창 최대소지량
	_uint m_iMaxAltAmmo;		//최대 소지가능 2차 탄환


public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

};


#endif // !__PLAYER_WEAPON_H__