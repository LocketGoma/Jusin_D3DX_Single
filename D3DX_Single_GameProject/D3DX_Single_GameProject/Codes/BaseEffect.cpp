#include "framework.h"
#include "BaseEffect.h"

#include "Transform.h"

CBaseEffect::CBaseEffect(_Device pDevice)
	: CGameObject(pDevice)
{
}

CBaseEffect::CBaseEffect(const CBaseEffect& other)
	: CGameObject(other)
{
}

void CBaseEffect::Set_Position(_vec3 vPos)
{
	m_pTransformCom->Set_Pos(vPos);
}

void CBaseEffect::Set_Size(_vec3 vSize)
{
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CBaseEffect::Get_Position()
{
	return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CBaseEffect::Get_Size()
{
	return m_pTransformCom->Get_TransformDescription().vScale;
}

void CBaseEffect::Set_Visible(_bool bState)
{
	m_bIsVisible = bState;
}

_bool CBaseEffect::Get_Visible()
{
	return m_bIsVisible;
}

void CBaseEffect::Set_Target(Engine::CDynamicMesh* pTarget)
{
	m_pTargetMesh = pTarget;
}

void CBaseEffect::Set_ParentMatrix(_mat& pMat)
{
	matParent = pMat;
}

void CBaseEffect::Set_UpParentMatrix(_mat& pMat)
{
	matUpParent = pMat;
}

void CBaseEffect::Free()
{
	Safe_Release(m_pBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	CGameObject::Free();
}
