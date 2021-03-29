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
	virtual void Shoot_Weapon() PURE;
	virtual void AltShoot_Weapon() PURE;
	virtual void Reload_Weapon() PURE;

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

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

};


#endif // !__PLAYER_WEAPON_H__