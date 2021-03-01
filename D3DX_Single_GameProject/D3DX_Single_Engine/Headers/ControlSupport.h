#pragma once

#ifndef __CONTROL_SUPPORT_COMPONENT_H__
#define __CONTROL_SUPPORT_COMPONENT_H__

#include "Engine_Include.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class CVIBuffer;
class CTransform;

class ENGINE_DLL CControlSupportUnit : public CComponent
{
private:
	explicit CControlSupportUnit(_Device pDevice);
	explicit CControlSupportUnit(const CControlSupportUnit& other);
	virtual ~CControlSupportUnit() = default;

public:
	HRESULT Ready_SupportUnit();

public:
	//지형에서의 기본 높이 구하기, pTerrainVTX가 터레인 버텍스 정보 가지고 오는놈임.
	_float Calculate_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVTX, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVTXInterval = 1);
	_vec3 Picking_Object(HWND hWnd, const CVIBuffer* pBuffer, const CTransform* pTransform);
	
private:
	_Device m_pDevice;

public:
	static CControlSupportUnit* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;


};

END_NAMESPACE

#endif // !__CONTROL_SUPPORT_COMPONENT_H__