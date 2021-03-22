#include "framework.h"
#include "NaviMeshController.h"

#include "Transform.h"

CNaviMeshController::CNaviMeshController(_Device pDevice)
	: CGameObject(pDevice)
{
}

CNaviMeshController::CNaviMeshController(const CNaviMeshController& other)
	:CGameObject(other)
	, m_pNaviMeshCom(other.m_pNaviMeshCom)
{
	m_bIsPrototype = false;
}

HRESULT CNaviMeshController::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CNaviMeshController::Ready_GameObject_Clone(void* pArg)
{
	return E_FAIL;
}

_int CNaviMeshController::Update_GameObject(const _float& fDeltaTime)
{
	return _int();
}

_int CNaviMeshController::LateUpdate_GameObject(const _float& fDeltaTime)
{
	if (m_bVisuable == true)
	{
		auto pManagement = Engine::CManagement::Get_Instance();
		if (nullptr == pManagement)
		{
			return MANAGER_OUT;
		}

		pManagement->Add_RenderList(Engine::RENDERID::RENDER_WIREFRAME, this);
	}

	return _int();
}

HRESULT CNaviMeshController::Render_GameObject(void)
{
	if (m_bVisuable == true)
	{
		m_pNaviMeshCom->Render_NaviMesh();
	}


	return S_OK;
}

_uint CNaviMeshController::Set_NaviMesh(const _tchar* meshName)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, meshName));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_NAVI", pComponent);

	return NO_EVENT;
}

HRESULT CNaviMeshController::Compare_NaviMove(std::map<const _tchar*, Engine::CLayer*>* pTargetScene)
{
	if (m_pNaviMeshCom == nullptr)
	{
		return E_FAIL;
	}

	for (auto& iter : *pTargetScene)
	{
		if (iter == *pTargetScene->end())
		{
			return E_FAIL;
		}
		if (iter.second == nullptr)
		{
			continue;
		}

		for (auto& targetObject : *(iter.second->Get_ObjectLayer()))
		{
			if (targetObject.second == nullptr)
			{
				continue;
			}

			Engine::CTransform* pTransform = dynamic_cast<Engine::CTransform*>(targetObject.second->Find_Component(L"Com_Transform", Engine::COMPONENT_ID::ID_DYNAMIC));
			
			if (pTransform != nullptr) {
				pTransform->Set_Pos(m_pNaviMeshCom->Compare_OnNaviMesh(&(pTransform->Get_Info(Engine::TRANSFORM_INFO::INFO_POS)), &(
					pTransform->Get_Info_RawData(Engine::TRANSFORM_INFO::INFO_POS))));
			}
		}
	}

	return S_OK;
}

void CNaviMeshController::Set_Position(_vec3 vPos)
{
	//¾È¾¸
}
void CNaviMeshController::Set_Size(_vec3 vSize)
{
	//¾È¾¸
}
_vec3 CNaviMeshController::Get_Position()
{
	return _vec3();
}
_vec3 CNaviMeshController::Get_Size()
{
	return _vec3();
}

void CNaviMeshController::Set_Visuable(_bool bState)
{
	m_bVisuable = bState;
}

HRESULT CNaviMeshController::Add_Component(void)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	// NaviMesh
	pComponent = m_pNaviMeshCom = dynamic_cast<Engine::CNaviMesh*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_MESH, L"Mesh_Navi"));	

	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[0].emplace(L"Com_NAVI", pComponent);

	return S_OK;
}

CNaviMeshController* CNaviMeshController::Create(_Device pDevice)
{
	CNaviMeshController* pInstance = new CNaviMeshController(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CNaviMeshController::Clone(void* pArg)
{
	CGameObject* pClone = new CNaviMeshController(*this);
	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CNaviMeshController");
	}

	if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
	{
		Safe_Release(pClone);
	}

	return pClone;
}

void CNaviMeshController::Free(void)
{
	Safe_Release(m_pNaviMeshCom);


	Engine::CGameObject::Free();
}

