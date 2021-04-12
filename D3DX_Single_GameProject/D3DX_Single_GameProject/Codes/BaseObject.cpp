#include "framework.h"
#include "BaseObject.h"

#include "Transform.h"
#include "ControlSupport.h"
#include "SphereCollider.h"


CBaseObject::CBaseObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_iWeight(0)
	, m_fLifeTime(-1.f)
	, m_fSpeed(0.f)
	, m_eForceType(eForceType::NONE)
	, m_fHitboxSize(1.f)
	, m_vStartPos(_vec3(0.f, 0.f, 0.f))
	, m_vDirection(_vec3(0.f, 0.f, 1.f))
	, eType(Engine::COLIDETYPE::COL_FALSE)
	, m_fGravitionPower(9.8f)
	, m_fGravitionSpeed(0.f)
	, m_bGravition(false)
{
	m_bIsPrototype = true;
}

CBaseObject::CBaseObject(const CBaseObject& other)
	: CGameObject(other)
	, m_iWeight(other.m_iWeight)
	, m_fLifeTime(other.m_fLifeTime)
	, m_fSpeed(other.m_fSpeed)
	, m_eForceType(other.m_eForceType)
	, m_fHitboxSize(other.m_fHitboxSize)
	, m_vStartPos(other.m_vStartPos)
	, m_vDirection(other.m_vDirection)
	, eType(Engine::COLIDETYPE::COL_FALSE)
	, m_fGravitionPower(other.m_fGravitionPower)
	, m_fGravitionSpeed(other.m_fGravitionSpeed)
	, m_bGravition(false)
{
	Safe_AddReference(m_pDevice);
	m_bIsPrototype = false;
}
void CBaseObject::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CBaseObject::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

void CBaseObject::Set_Direction(_vec3 vDir)
{	
	m_vDirection = vDir;
	//m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &vDir);
}

void CBaseObject::Set_ObjectType(eForceType eType)
{
	m_eForceType = eType;
}

void CBaseObject::Set_SpeedLockState(_bool bLock)
{
	m_bSpeedLock = bLock;
}

_vec3 CBaseObject::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CBaseObject::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

_vec3 CBaseObject::Get_Direction()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK);
}

const eForceType CBaseObject::Get_ObjectType()
{
	return m_eForceType;
}

_bool CBaseObject::Get_SpeedLockState()
{
	return m_bSpeedLock;
}

Engine::CControlSupportUnit* CBaseObject::Get_SupportUnit()
{
	return m_pSupportCom;
}

const _float CBaseObject::Get_LifeTime()
{
	return _float();
}

void CBaseObject::Set_LifeTime(_float fTime)
{
	m_fLifeTime = fTime;
}

const _float CBaseObject::Get_Speed()
{
	return m_fSpeed;
}

void CBaseObject::Set_Speed(_float fSpeed)
{
	m_fSpeed = fSpeed;
}

void CBaseObject::Set_GravitionPower(_float fGravition)
{
	m_fGravitionPower = fGravition;
}

void CBaseObject::Set_ClearGSpeed(_float fClearHeight)
{
	_vec3 vPos = m_pTransformCom->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS);
	vPos.y = fClearHeight;
	m_pTransformCom->Set_Pos(vPos);

	m_fGravitionSpeed = 0.f;

	m_bGravition = false;
}

void CBaseObject::Set_GravitionDrop()
{
	m_bGravition = true;
}

const Engine::CTransform* CBaseObject::Get_Transform()
{
	return m_pTransformCom;
}

void CBaseObject::Free(void)
{
	if (m_bIsPrototype == false)
	{
		Safe_Release(m_pDevice);
	}

	Engine::CGameObject::Free();
}
