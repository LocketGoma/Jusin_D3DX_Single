#pragma once
#ifndef __GRAPHIC_RESOURCE_H__
#define __GRAPHIC_RESOURCE_H__

#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGraphicResources : public CComponent
{
protected:
	explicit CGraphicResources(_Device pDevice);
	explicit CGraphicResources(const CGraphicResources& other);
	virtual ~CGraphicResources() = default;

protected:
	_Device		m_pDevice;
	
public:
	virtual void Free();

};

END_NAMESPACE

#endif // !__GRAPHIC_RESOURCE_H__
