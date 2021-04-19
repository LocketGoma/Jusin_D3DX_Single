#include "framework.h"
#include "StaticMesh.h"
#include "Transform.h"

#include "MapObject.h"
#include "Transform.h"
#include "Shader.h"

CMapObject::CMapObject(_Device pDevice)
	: CGameObject(pDevice)
	, m_pTransformCom(nullptr)
	, m_pMeshCom(nullptr)
{
}

CMapObject::CMapObject(const CMapObject& other)
	: CGameObject(other)
	, m_pTransformCom(other.m_pTransformCom)
	, m_pMeshCom(other.m_pMeshCom)
{

}

HRESULT CMapObject::Set_MapName(const _tchar* pMapName)
{
	if (lstrlen(pMapName) == 0)
		return E_FAIL;

	lstrcpy(m_pMapName, pMapName);

	return S_OK;
}

const _tchar* CMapObject::Get_MapName()
{
	return m_pMapName;
}

void CMapObject::Set_Position(_vec3 vPos)
{
	if (m_pTransformCom!=nullptr)
	m_pTransformCom->Set_Pos(vPos);
}

void CMapObject::Set_Size(_vec3 vSize)
{
	if (m_pTransformCom != nullptr)
	m_pTransformCom->Set_Scale(vSize);
}

_vec3 CMapObject::Get_Position()
{
	if (m_pTransformCom != nullptr)
		return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
	else
		return _vec3(0.f, 0.f, 0.f);
}

_vec3 CMapObject::Get_Size()
{
	if (m_pTransformCom != nullptr)
		return m_pTransformCom->Get_TransformDescription().vScale;
	else
		return _vec3(0.f, 0.f, 0.f);
}

HRESULT CMapObject::Setup_ConstantTable(LPD3DXEFFECT& pEffect)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement || pEffect == nullptr)
	{
		return E_FAIL;
	}

	_mat	matWorld, matView, matProj;

	matWorld = m_pTransformCom->Get_TransformDescription().matWorld;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	return S_OK;
}

void CMapObject::Free(void)
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pMeshCom);
	Safe_Release(m_pTransformCom);

	Engine::CGameObject::Free();
}
