#pragma once

#ifndef __LIGHTING_MANAGER_H__
#define __LIGHTING_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Light.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CLightingManager : public CBase
{
	DECLARE_SINGLETON(CLightingManager)

private:
	explicit CLightingManager();
	virtual ~CLightingManager() = default;

public:
	HRESULT Ready_Light();
	HRESULT Set_Light(const _uint& iIndex, _bool bSetType);

private:
	std::vector<CLight*>	m_LightVector;

private:
	virtual void Free(void);


};


#endif // !__LIGHTING_MANAGER_H__