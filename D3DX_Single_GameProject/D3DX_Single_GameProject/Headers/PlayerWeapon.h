#pragma once
#ifndef __PLAYER_WEAPON_H__
#define __PLAYER_WEAPON_H__

#include "GameObject.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)
class CDynamicMesh;
END_NAMESPACE

class CEffectMuzzle;

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

//����/����
public:
	virtual void Draw_Weapon() PURE;		//���� ������
	virtual void Shoot_Weapon() PURE;		//1�����
	virtual void AltShoot_Weapon() PURE;	//2�����
	virtual bool Reload_Weapon();			//������
	virtual void Release_Weapon() PURE;		//�������
	virtual void Holster_Weapon() PURE;		//���� ����ֱ�

	_uint Get_Damage_Primary();
	_uint Get_Damage_Secondery();

//ź Get/Set
public:
	_uint Get_RemainAmmo();				//���� ���� źȯ
	_uint Get_MagAmmo();				//źâ�� ���� źȯ
	_uint Get_RemainAltAmmo();			//���� ���� 2�� źȯ
	void Add_Ammo(_uint _iAmmo);		//źȯ �߰�
	void Add_AltAmmo(_uint _iAmmo);		//2�� źȯ �߰�

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

public:
	void Set_Animation(int iNumber);
	bool Is_End_Animation();
	int Get_VertexNumber();

protected:
	Engine::CDynamicMesh* m_pMeshCom;
	_uint m_iMagAmmo;			//źâ ���� źȯ
	_uint m_iMainAmmo;			//������ źȯ
	_uint m_iAltAmmo;			//������ 2��źȯ
	_uint m_iMaxAmmo;			//�ִ� �������� źȯ
	_uint m_iMaxMagAmmo;		//źâ �ִ������
	_uint m_iMaxAltAmmo;		//�ִ� �������� 2�� źȯ
	_uint m_iROF;				//�д� �ִ� ��ݰ��� �� �� (=�����)
	_float m_fFireInterval;		//1�ߴ� ��Ÿ��		= �ڵ� ���
	_float m_fAltFireInterval;	//Ư����� ��Ÿ��
	_float m_fNowFItime;		//���� ��Ÿ��
	_float m_fNowAFItime;		//���� ��Ÿ��

	_uint m_iPriDamage;
	_uint m_iSecDamage;

	_bool m_bFire;
	_bool m_bAltFire;

	_float m_fTime;				//��ŸŸ�� ������

	CEffectMuzzle* m_pEffect = nullptr;		//��������Ʈ
	CEffectMuzzle* m_pAltEffect = nullptr;	//����Ʈ

public:
	virtual CGameObject* Clone(void* pArg = nullptr) PURE;
	virtual void Free() override;

};


#endif // !__PLAYER_WEAPON_H__