#include "framework.h"
#include "StaticObject.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "ControlSupport.h"
#include "SphereCollider.h"


CStaticObject::CStaticObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_iWeight(0)
	, m_fLifeTime(-1.f)
	, m_fSpeed(0.f)
	, m_eForceType(eForceType::NONE)
	, m_fHitboxSize(1.f)
{
}

CStaticObject::CStaticObject(const CStaticObject& other)
	: CGameObject(other)
	, m_iWeight(other.m_iWeight)
	, m_fLifeTime(other.m_fLifeTime)
	, m_fSpeed(other.m_fSpeed)
	, m_eForceType(other.m_eForceType)
	, m_fHitboxSize(other.m_fHitboxSize)
{
	Safe_AddReference(m_pDevice);
}

void CStaticObject::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CStaticObject::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CStaticObject::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CStaticObject::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

const _uint CStaticObject::Get_Weight()
{
	return m_iWeight;
}

void CStaticObject::Set_Weight(_uint iWeight)
{
	m_iWeight = iWeight;
}

const eForceType CStaticObject::Get_ObjectType()
{
	return m_eForceType;
}

void CStaticObject::Set_ObjectType(eForceType eType)
{
	m_eForceType = eType;
}

const _float CStaticObject::Get_LifeTime()
{
	return m_fLifeTime;
}

void CStaticObject::Set_LifeTime(_float fTime)
{
	m_fLifeTime = fTime;
}

const _float CStaticObject::Get_Speed()
{
	return m_fSpeed;
}

void CStaticObject::Set_Speed(_float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CStaticObject::Free(void)
{
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pDevice);
	}

	CGameObject::Free();
}

