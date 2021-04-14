#include "framework.h"
#include "StaticNormalObject.h"

#include "StaticMesh.h"
#include "Transform.h"
#include "SphereCollider.h"
#include "ControlSupport.h"

CStaticNormalObject::CStaticNormalObject(_Device pDevice, const _tchar* pMeshName, _float fHitBoxSize, _float fWeight)
	: CStaticObject(pDevice)
{
	
	if (pMeshName!=nullptr)
		lstrcpy(m_pMeshName, pMeshName);

	m_fWeight = fWeight;
	m_fLifeTime = -1.f;
	m_fSpeed = 0.f;
	m_eForceType = eForceType::PULL;
	m_fHitboxSize = fHitBoxSize;

	m_vStartPos = _vec3(0.f, 0.f, 0.f);
	m_vDirection = _vec3(0.f, 0.f, 1.f);

}

CStaticNormalObject::CStaticNormalObject(const CStaticNormalObject& other)
	: CStaticObject(other)
{
	if (m_pMeshName != nullptr && lstrlen(other.m_pMeshName)>2)
		lstrcpy(m_pMeshName, other.m_pMeshName);

}

HRESULT CStaticNormalObject::Ready_GameObject(_uint iTexNumber)
{
	return S_OK;
}

HRESULT CStaticNormalObject::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransformCom->Set_Info(Engine::TRANSFORM_INFO::INFO_LOOK, &m_vDirection);

	return S_OK;
}

_int CStaticNormalObject::Update_GameObject(const _float& fDeltaTime)
{
	Engine::CGameObject::Update_GameObject(fDeltaTime);
	m_pTransformCom->Set_Scale(RESET_VECTOR);
	m_pTransformCom->Update_Component();


	m_vDirection = m_vDirection + _vec3(0.f, -0.1f, 0.f);
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	m_fSpeed *= D3DXVec2Length(&_vec2(m_vDirection.x, m_vDirection.z));
	m_fSpeed /= m_fWeight;
	m_vDirection = _vec3(m_vDirection.x, 0.f, m_vDirection.z);

	m_fGravitionSpeed += m_fGravitionPower * fDeltaTime;


	
	
	return NO_EVENT;
}

_int CStaticNormalObject::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	m_pTransformCom->Move_Pos(&m_vDirection, m_fSpeed, fDeltaTime);
	m_pTransformCom->Move_Pos(&_vec3(0.0f, -1.f, 0.0f), m_fGravitionSpeed, fDeltaTime);

	m_pTransformCom->Update_Component();

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_NOALPHA, this);

	return NO_EVENT;
}

HRESULT CStaticNormalObject::Render_GameObject(void)
{

	if (FAILED(CGameObject::Render_GameObject()))
		return E_FAIL;

	_mat matWorld = m_pTransformCom->Get_TransformDescription().matWorld;

	m_pTransformCom->Set_Scale(BASE_ENEMY_REDUCION_VECTOR);
	m_pTransformCom->Update_Component();
	m_pTransformCom->LateUpdate_Component(0.f);


	if (m_pMeshCom != nullptr)
	{
		m_pMeshCom->Render_Meshes();
	}


	m_pColliderCom->Render_Collider(eType, &matWorld, g_bViewCollider);

	return S_OK;
}

HRESULT CStaticNormalObject::Add_Component()
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}
	Engine::CComponent* pComponent = nullptr;

	// Transform
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	if (m_pMeshName != nullptr && lstrlen(m_pMeshName) > 2)
	{

		pComponent = m_pMeshCom = dynamic_cast<Engine::CStaticMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, m_pMeshName));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Mesh", pComponent);

	}

	//서포트 컴포넌트
	pComponent = m_pSupportCom = Engine::CControlSupportUnit::Create(m_pDevice);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Support", pComponent);

	//콜라이더
	pComponent = m_pColliderCom = Engine::CSphereCollider::Create(m_pDevice, &_vec3(0.f, 0.f, 0.f), m_fHitboxSize);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Collider", pComponent);

	return S_OK;
}

CStaticNormalObject* CStaticNormalObject::Create(_Device pDevice, const _tchar* pMeshName, _float fHitBoxSize, _float fWeight)
{
	CStaticNormalObject* pInstance = new CStaticNormalObject(pDevice,pMeshName,fHitBoxSize, fWeight);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
	

	return nullptr;
}

Engine::CGameObject* CStaticNormalObject::Clone(void* pArg)
{
	CStaticNormalObject* pClone = new CStaticNormalObject(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStaticNormalObject");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CStaticNormalObject::Free()
{
	CStaticObject::Free();
}
