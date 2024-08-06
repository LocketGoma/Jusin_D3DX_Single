#pragma once
#ifndef __MESH_H__
#define __MESH_H__

#include "Engine_Include.h"
#include "GraphicResources.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CMesh : public CGraphicResources
{
protected:
	explicit CMesh(_Device pDevice);
	explicit CMesh(const CMesh& other);
	virtual ~CMesh() = default;

public:
	virtual CComponent* Clone(void* pArg = nullptr) PURE;
	virtual void		Free(void);


};

END_NAMESPACE

#endif // !__MESH_H__