#pragma once

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Engine_Include.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CLight : public CComponent
{
private:
	explicit CLight(_Device pDevice);	
	explicit CLight(const CLight& other);
	virtual ~CLight()=default;

public:
	HRESULT Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iLndex);


	static CLight* Create(_Device pDevice, const D3DLIGHT9* pLightInfo, const _uint& ilight);
	// CComponent을(를) 통해 상속됨
	virtual CComponent* Clone(void* pArg = nullptr) override;

	virtual void Free(void) override;


private:
	_Device		m_pDevice;
	D3DLIGHT9	m_tLightInfo;
	_uint		m_iIndex;

};

END_NAMESPACE


#endif // !__LIGHT_H__