#include "framework.h"
#include "..\Headers\DynamicObject.h"

void CDynamicObject::Free(void)
{
	CGameObject::Free();
}

CDynamicObject::CDynamicObject(_Device pDevice)
	: CGameObject(pDevice)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& other)
	: CGameObject(other)
{
	Safe_AddReference(m_pDevice);
}
