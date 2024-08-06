#pragma once

#ifndef __COMPONENET_H__
#define __COMPONENET_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent(void);
	explicit CComponent(const CComponent& other);
	virtual ~CComponent(void);

public:
	virtual _int	Update_Component(const _float& fDeltaTime);
	virtual _int	LateUpdate_Component(const _float& fDeltaTime);

public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;

protected:
	virtual void Free(void) PURE;

	_bool m_bIsPrototype;
};

END_NAMESPACE

#endif // !__COMPONENET_H__