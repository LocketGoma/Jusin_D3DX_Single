#pragma once

#ifndef __CONTROL_SUPPORT_COMPONENT_H__
#define __CONTROL_SUPPORT_COMPONENT_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class CVIBuffer;
class CTransform;
class CVTXTerrain;
class CStaticMesh;
class CDynamicMesh;
class CCollider;
class CSphereCollider;

class ENGINE_DLL CControlSupportUnit : public CComponent
{
private:
	typedef	struct tagOBB
	{
		_vec3		vPoint[8];
		_vec3		vCenter;
		_vec3		vProjAxis[3];	// �������� �� ������ ���� ������ ���� ����
		_vec3		vAxis[3];		// �ڽ��� ������ �� ����

	}OBB;

private:
	explicit CControlSupportUnit(_Device pDevice);
	explicit CControlSupportUnit(const CControlSupportUnit& other);
	virtual ~CControlSupportUnit() = default;

public:
	HRESULT Ready_SupportUnit();

public:
	//���������� �⺻ ���� ���ϱ�, pTerrainVTX�� �ͷ��� ���ؽ� ���� ������ ���³���.
	_float Calculate_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVTX, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVTXInterval = 1);	
	_bool Picking_Object_Static(HWND hWnd, const CStaticMesh* pMesh, const CTransform* pTransform);
	_bool Picking_Object_Dynamic(HWND hWnd, const CDynamicMesh* pMesh, const CTransform* pTransform);
	_bool Picking_Object_Collider(HWND hWnd, const CSphereCollider* pMesh, const CTransform* pTransform);
	_vec3 Picking_Object(HWND hWnd, const CStaticMesh* pMesh, const CTransform* pTransform);
	_vec3 Picking_Terrain(HWND hWnd, const CVTXTerrain* pBuffer, const CTransform* pTransform);	

	_bool Collision_Picking(HWND hWnd, CCollider* pCollider, CTransform* pTransform);

	_bool Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _mat* pDestWorld, const _vec3* pSourceMin, const _vec3* pSourceMax, const _mat* pSourceWorld);

	_bool Collision_Sphere(const _vec3* pDestPos, const _float fDestRadius, const _vec3* pSourcePos, const _float fSourceRadius);

//����� �����ϴ°�
public:
	_float Get_Distance();
	
private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

private:
	_Device m_pDevice;
	
	_float m_fDistance;		//�Ÿ�

public:
	static CControlSupportUnit* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END_NAMESPACE

#endif // !__CONTROL_SUPPORT_COMPONENT_H__