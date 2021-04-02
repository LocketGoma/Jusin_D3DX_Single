#include "framework.h"
#include "DynamicObject.h"

#include "ControlSupport.h"
#include "Transform.h"
#include "DynamicMesh.h"

CDynamicObject::CDynamicObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_fMoveSpeed(5.f)
	, m_fAnimationSpeed(1.0f)
	, m_fAttackInterval(1.f)
	, m_fRotate(0.f)
	, m_iMaxHP(10)
	, m_iHP(10)
	, m_iDamage(5)
	, m_fRecognizeRange(30.f)
	, m_fMoveRange(20.f)
	, m_fAttackRange(10.f)
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
	, m_fRecognizeRange(other.m_fRecognizeRange)
	, m_fMoveRange(other.m_fMoveRange)
	, m_fAttackRange(other.m_fAttackRange)
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

bool CDynamicObject::Hit_Attack(_uint iDamage)
{
	m_iHP -= iDamage;

	if (m_iHP <= 0)
	{
		return true;
	}
	return false;
}

const _float CDynamicObject::Get_RecogRange()
{
	return m_fRecognizeRange;
}

const _float CDynamicObject::Get_MoveRange()
{
	return m_fMoveRange;
}

const _float CDynamicObject::Get_AttackRange()
{
	return m_fAttackRange;
}

const Engine::CTransform* CDynamicObject::Get_Transform()
{
	return m_pTransformCom;
}

void CDynamicObject::Free(void)
{
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pDevice);
	}

	CGameObject::Free();
}

void CDynamicObject::Do_Anichange(_uint iAnimation)
{
	m_pMeshCom->Set_AnimationSet(iAnimation);
}

void CDynamicObject::Check_Hit(_bool bForce, _uint iDamage)
{
	//if(bForce)
	//{
	//		Hit_Attack(iDamage);
	//}

	if (m_pSupportCom->Picking_Object_Dynamic(g_hWnd, m_pMeshCom, m_pTransformCom))
	{
		eType = Engine::COLIDETYPE::COL_TRUE;
		Hit_Attack(iDamage);
	}
	else
	{
		eType = Engine::COLIDETYPE::COL_FALSE;
	}



}
