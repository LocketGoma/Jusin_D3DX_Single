#include "framework.h"
#include "DynamicObject.h"


CDynamicObject::CDynamicObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_fMoveSpeed(10.f)
	, m_fAnimationSpeed(1.0f)
	, m_fAttackInterval(1.f)
	, m_fRotate(0.f)
	, m_iMaxHP(10)
	, m_iHP(10)
	, m_iDamage(5)
{
}

CDynamicObject::CDynamicObject(const CDynamicObject& other)
	: CGameObject(other)
	, m_fMoveSpeed(other.m_fMoveSpeed)
	, m_fAnimationSpeed(other.m_fAnimationSpeed)
	, m_fAttackInterval(other.m_fAttackInterval)
	, m_fRotate(other.m_fRotate)
	, m_iMaxHP(other.m_iMaxHP)
	, m_iHP(other.m_iHP)
	, m_iDamage(other.m_iDamage)
{
	Safe_AddReference(m_pDevice);
}

_uint CDynamicObject::Get_Damage()
{
	return m_iDamage;
}

_uint CDynamicObject::Get_NowHP()
{
	return m_iHP;
}

void CDynamicObject::Hit_Attack(_uint iDamage)
{
	m_iHP -= iDamage;

	if (m_iHP <= 0)
	{
		m_bDead = true;
	}
}

void CDynamicObject::Free(void)
{
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pDevice);
	}

	CGameObject::Free();
}
