#include "framework.h"
#include "DynamicObject.h"


CDynamicObject::CDynamicObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_fMoveSpeed(10.f)
	, m_fAnimationSpeed(1.0f)
	, m_fAttackInterval(1.f)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& other)
	: CGameObject(other)
	, m_fMoveSpeed(other.m_fMoveSpeed)
	, m_fAnimationSpeed(other.m_fAnimationSpeed)
	, m_fAttackInterval(other.m_fAttackInterval)
{
	Safe_AddReference(m_pDevice);
}

void CDynamicObject::Free(void)
{
	CGameObject::Free();
}
