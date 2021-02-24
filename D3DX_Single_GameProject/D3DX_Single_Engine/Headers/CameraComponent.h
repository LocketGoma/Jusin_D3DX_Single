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

public:
	virtual _int	Update_Component(const _float& fTimeDelta);
	virtual _int	LateUpdate_Component(const _float& fTimeDelta);

public:
	// CComponent을(를) 통해 상속됨
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;

protected:
	CAMERA_DESC m_CameraDesc;

	_Device		m_pDevice;

};

END_NAMESPACE

#endif // !__CAMERA_COMPONENT_H__