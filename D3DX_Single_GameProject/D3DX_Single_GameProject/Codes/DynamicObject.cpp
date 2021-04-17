#include "framework.h"
#include "DynamicObject.h"

#include "ControlSupport.h"
#include "Transform.h"
#include "DynamicMesh.h"

CDynamicObject::CDynamicObject(_Device pDevice)
	: CBaseObject(pDevice)
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
	, m_fRotateSpeed(1.0f)
	, m_bEndChecker(false)
	, m_bStartMove(false)
	, m_vCorePos(_vec3(0.f,0.f,0.f))
	, m_bState(true)
	, m_eChannel(Engine::SOUND_CHANNELID::ENEMY)
{
	m_iHP = m_iMaxHP;
	m_eForceType = eForceType::NONE;
}

CDynamicObject::CDynamicObject(const CDynamicObject& other)
	: CBaseObject(other)
	, m_fMoveSpeed(other.m_fMoveSpeed)
	, m_fAnimationSpeed(other.m_fAnimationSpeed)
	, m_fAttackInterval(other.m_fAttackInterval)
	, m_fRotate(other.m_fRotate)
	, m_iMaxHP(other.m_iMaxHP)
	, m_iHP(other.m_iMaxHP)
	, m_iDamage(other.m_iDamage)
	, m_fRecognizeRange(other.m_fRecognizeRange)
	, m_fMoveRange(other.m_fMoveRange)
	, m_fAttackRange(other.m_fAttackRange)
	, m_fNowAttackTime(0.f)
	, m_fRotateSpeed(other.m_fRotateSpeed)
	, m_bEndChecker(false)
	, m_bStartMove(false)
	, m_vCorePos(other.m_vCorePos)
	, m_bState(other.m_bState)
	, m_eChannel(other.m_eChannel)
{
}
//�� �����ָ� �ִϸ��̼ǰ� �浹������ ��Ȯ�� ��.
void CDynamicObject::Force_Update_Animation()
{
	m_pMeshCom->Play_AnimationSet(0.f);
	m_pMeshCom->Update_Meshes();
	//m_pTransformCom->Update_Component(0.f);
}

_bool CDynamicObject::End_Animation_State()
{
	return m_pMeshCom->End_AnimationSet();
}


_uint CDynamicObject::Get_Damage()
{
	if (m_bAttackHitEnable == false)
	{
		return 0;
	}
	else
	{
		m_bAttackHitEnable = false;
	}

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

void CDynamicObject::Set_StartMove()
{
	m_bStartMove = true;
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

const _float CDynamicObject::Get_CollideRange()
{
	return m_fHitboxSize;
}

const _vec3 CDynamicObject::Get_CorePos()
{
	return m_vCorePos;
}

_bool CDynamicObject::Check_RayCollision()
{
	return m_pSupportCom->Picking_Object_Dynamic(g_hWnd, m_pMeshCom, m_pTransformCom);
}

_bool CDynamicObject::Check_Attack_Collide(const _vec3* pSourcePos, const _float fSourceRadius)
{
	_bool bReturn = m_pSupportCom->Collision_Sphere(&(this->Get_Position()), this->m_fHitboxSize, pSourcePos, fSourceRadius);

	bReturn == true ? eType = Engine::COLIDETYPE::COL_TRUE : eType = Engine::COLIDETYPE::COL_FALSE;

	return bReturn;
}

_bool CDynamicObject::Check_RayCollision_By_CollisionSphere()
{
	if (m_pSupportCom->Picking_Object_Collider(g_hWnd, m_pColliderCom, m_vCorePos))
	{
		eType = Engine::COLIDETYPE::COL_INRANGE;
		return true;
	}
	else
	{
		eType = Engine::COLIDETYPE::COL_FALSE;
		return false;
	}
}

_bool CDynamicObject::Get_State()
{
	return m_bState;
}

void CDynamicObject::Set_Speed(_float fSpeed)
{
	m_fImForcePower = fSpeed;

}

void CDynamicObject::Set_Direction(_vec3 vDir)
{
	m_vImForceDirection = vDir;
}



void CDynamicObject::Free(void)
{
	CBaseObject::Free();
}

void CDynamicObject::Do_Dead(_float fDeltaTime)
{
	this->Set_Dead();
}

void CDynamicObject::Do_Anichange(_uint iAnimation)
{
	m_pMeshCom->Set_AnimationSet(iAnimation);
}

void CDynamicObject::Check_Hit(_bool bForce, _uint iDamage)
{
	if (m_pSupportCom->Picking_Object_Dynamic(g_hWnd, m_pMeshCom, m_pTransformCom))
	{
		eType = Engine::COLIDETYPE::COL_TRUE;
		Hit_Attack(iDamage);
	}
	else if (eType != Engine::COLIDETYPE::COL_INRANGE)
	{
		eType = Engine::COLIDETYPE::COL_FALSE;
	}
}

_vec3 CDynamicObject::Calc_Position_At_Mesh()
{
	_vec3 vNowPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);

	_vec3 vGab = m_vCorePos - m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
	
	vGab.y = 0.f;

	_vec3 vCalivPos = m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS) - vGab;

	return vCalivPos;
}

_vec3 CDynamicObject::Get_Position_At_Mesh()
{
	return m_vCorePos;
}
