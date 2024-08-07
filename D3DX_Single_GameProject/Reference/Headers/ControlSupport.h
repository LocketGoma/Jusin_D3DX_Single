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
		_vec3		vProjAxis[3];	// 중점에서 각 면으로 뻗어 나가는 방향 벡터
		_vec3		vAxis[3];		// 박스에 평행한 축 벡터

	}OBB;

private:
	explicit CControlSupportUnit(_Device pDevice);
	explicit CControlSupportUnit(const CControlSupportUnit& other);
	virtual ~CControlSupportUnit() = default;

public:
	HRESULT Ready_SupportUnit();

public:
	//지형에서의 기본 높이 구하기, pTerrainVTX가 터레인 버텍스 정보 가지고 오는놈임.
	_float Calculate_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVTX, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVTXInterval = 1);	
	_bool Picking_Object_Static(HWND hWnd, const CStaticMesh* pMesh, const CTransform* pTransform);
	_bool Picking_Object_Dynamic(HWND hWnd, const CDynamicMesh* pMesh, const CTransform* pTransform);
	_bool Picking_Object_Collider(HWND hWnd, const CSphereCollider* pMesh);
	_bool Picking_Object_Collider(HWND hWnd, const CSphereCollider* pMesh, const _vec3& pVPos);
	_bool Picking_Object_Collider(HWND hWnd, const CSphereCollider* pMesh, const CTransform* pTransform);
	_vec3 Picking_Object(HWND hWnd, const CStaticMesh* pMesh, const CTransform* pTransform);
	_vec3 Picking_Terrain(HWND hWnd, const CVTXTerrain* pBuffer, const CTransform* pTransform);	

	_bool Collision_Picking(HWND hWnd, CCollider* pCollider, CTransform* pTransform);

	_bool Collision_RayPick_Dynamic(_vec3 vDir, _vec3 vPos, const CDynamicMesh* pMesh, const CTransform* pTransform);

	_bool Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _mat* pDestWorld, const _vec3* pSourceMin, const _vec3* pSourceMax, const _mat* pSourceWorld);

	_bool Collision_Sphere(const _vec3* pDestPos, const _float fDestRadius, const _vec3* pSourcePos, const _float fSourceRadius);

//결과물 리턴하는곳
public:
	_float Get_Distance();
	_vec3 Get_Position();

	_vec3 Reflection(_vec3 vDir, _vec3 vUp);
	
private:
	void		Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax);
	void		Set_Axis(OBB* pObb);

private:
	_Device m_pDevice;
	
	_float m_fDistance;		//거리
	_vec3 m_vPosition;

public:
	static CControlSupportUnit* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END_NAMESPACE

#endif // !__CONTROL_SUPPORT_COMPONENT_H__