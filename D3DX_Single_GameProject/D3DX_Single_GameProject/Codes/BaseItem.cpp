#include "framework.h"
#include "BaseItem.h"



CBaseItem::CBaseItem(_Device pDevice)
	: CStaticObject(pDevice)
	, m_iAmount(0)
{
}

CBaseItem::CBaseItem(const CBaseItem& other)
	: CStaticObject(other)
	, m_iAmount(other.m_iAmount)
{
}

_uint CBaseItem::Get_Amount()
{
	return m_iAmount;
}

void CBaseItem::Free(void)
{
	CStaticObject::Free();
}
