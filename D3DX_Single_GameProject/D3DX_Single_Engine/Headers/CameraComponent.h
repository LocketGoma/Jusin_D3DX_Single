#pragma once

#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CCameraComponent : public CComponent
{
public:
	typedef struct tagCameraDescription
	{
		_mat matView;
		_float3 vEye, vAt, vUp;

		_mat matProj;
		_float fFovY, fAspect, fZNear, fZFar;

		_mat matOrtho;
	}CAMERA_DESC;

protected:
	explicit CCameraComponent(_Device pDevice);
	explicit CCameraComponent(const CCameraComponent& other);
	virtual ~CCameraComponent() = default;

public:
	const CAMERA_DESC& Get_CameraDesc() const;
	void Set_CameraDesc(CAMERA_DESC& cDesc);
	void Set_ViewVector(_vec3 vEye, _vec3 vAt, _vec3 vUp);
	void Set_Projection(_float fFovY, _float fAspect, _float fZnear = 1.f, _float fZFar = 1000.f);
	//void Set_Ortho(_vec3 vScale, _vec3 vPos);

	
	void Set_Ortho(_bool bUse);

public:	
	virtual HRESULT Ready_Camera();
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

public:
	// CComponent을(를) 통해 상속됨
	static CCameraComponent* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;

protected:
	CAMERA_DESC m_CameraDesc;

	_Device		m_pDevice;

	_bool		m_bUseOrtho;

};

END_NAMESPACE

#endif // !__CAMERA_COMPONENT_H__