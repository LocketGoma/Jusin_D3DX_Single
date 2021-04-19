#include "framework.h"
#include "StaticObject.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "ControlSupport.h"
#include "SphereCollider.h"


CStaticObject::CStaticObject(_Device pDevice)
	: CBaseObject(pDevice)
{
}

CStaticObject::CStaticObject(const CStaticObject& other)
	: CBaseObject(other)
{
}

const _float CStaticObject::Get_Weight()
{
	return m_fWeight;
}

void CStaticObject::Set_Weight(_float fWeight)
{
	m_fWeight = fWeight;
}

_bool CStaticObject::Check_RayCollision()
{
	return m_pSupportCom->Picking_Object_Static(g_hWnd,m_pMeshCom,m_pTransformCom);
}

_bool CStaticObject::Check_RayCollision_By_CollisionSphere()
{
	if (m_pSupportCom->Picking_Object_Collider(g_hWnd, m_pColliderCom))
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

void CStaticObject::Free(void)
{
	CBaseObject::Free();
}

HRESULT CStaticObject::Setup_ConstantTable(LPD3DXEFFECT& pEffect)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement || pEffect == nullptr)
	{
		return E_FAIL;
	}

	_matrix			matWorld, matView, matProj;

	matWorld = m_pTransformCom->Get_TransformDescription().matWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

