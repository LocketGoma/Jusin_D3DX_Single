#include "framework.h"
#include "NaviMeshController.h"

#include "Transform.h"
#include "DynamicObject.h"

#include "iostream"
#include "json.h"

CNaviMeshController::CNaviMeshController(_Device pDevice)
	: CGameObject(pDevice)
{
	m_bVisuable = true;
}

CNaviMeshController::CNaviMeshController(const CNaviMeshController& other)
	:CGameObject(other)
	, m_pNaviMeshCom(other.m_pNaviMeshCom)
{
	m_bIsPrototype = false;


}

HRESULT CNaviMeshController::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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
		Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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
		m_pNaviMeshCom->Render_NaviMesh(g_bViewNavimesh);
	}
	return S_OK;
}

_uint CNaviMeshController::Set_NaviMesh(const _tchar* meshName)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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

HRESULT CNaviMeshController::Set_NaviMesh_From_File(const _tchar* pFilePath)
{
	Clear_NaviMesh();

	std::unique_ptr<std::ifstream> inputFile;
	inputFile = std::make_unique<std::ifstream>(pFilePath);

	if (inputFile->is_open())
	{
		std::string sBuffer;
		sBuffer.assign(std::istreambuf_iterator<char>(*inputFile), std::istreambuf_iterator<char>());

		Json::CharReaderBuilder jBuilder;
		std::unique_ptr<Json::CharReader> jReader(jBuilder.newCharReader());
		Json::Value jRoot;
		JSONCPP_STRING err;

		if (jReader->parse(sBuffer.c_str(), sBuffer.c_str() + sBuffer.length(), &jRoot, &err) == false)
		{
			inputFile->close();
			return E_FAIL;
		}
		_uint iCount = 0;
		_uint iTotal = 0;
		Json::Value size = jRoot["CellCount"];
		std::string sSize = size.asString();
		iTotal = atoi(sSize.c_str());

		Json::Value CellParse = jRoot["Cell"];

		for (auto cell = CellParse.begin(); cell != CellParse.end(); cell++)
		{
			_vec3 NodeA, NodeB, NodeC;


			Json::Value jNodeA, jNodeB, jNodeC;
			jNodeA = (*cell)["Node"][0];
			jNodeB = (*cell)["Node"][1];
			jNodeC = (*cell)["Node"][2];

			NodeA.x = (float)(atof((jNodeA)["X"].asString().c_str()));
			NodeA.y = (float)(atof((jNodeA)["Y"].asString().c_str()));
			NodeA.z = (float)(atof((jNodeA)["Z"].asString().c_str()));

			NodeB.x = (float)(atof((jNodeB)["X"].asString().c_str()));
			NodeB.y = (float)(atof((jNodeB)["Y"].asString().c_str()));
			NodeB.z = (float)(atof((jNodeB)["Z"].asString().c_str()));

			NodeC.x = (float)(atof((jNodeC)["X"].asString().c_str()));
			NodeC.y = (float)(atof((jNodeC)["Y"].asString().c_str()));
			NodeC.z = (float)(atof((jNodeC)["Z"].asString().c_str()));

			m_pNaviMeshCom->Add_NaviCell(NodeA, NodeB, NodeC);
		}
	}
	else
	{
		inputFile->close();

		PRINT_LOG(L"Error", L"NaviMesh is not Exist");
		return E_FAIL;
	}
	inputFile->close();

	return S_OK;
}

void CNaviMeshController::Set_NowScene(Engine::CScene* pScene)
{
	m_pScene = pScene;
}

_uint CNaviMeshController::Get_NowIndex(const _vec3* vPos)
{
	return m_pNaviMeshCom->Caculate_Index(vPos);
}

HRESULT CNaviMeshController::Compare_NaviMove(Engine::CLayer* pTargetLayer)
{
	if (m_pNaviMeshCom == nullptr || pTargetLayer == nullptr)
	{
		return E_FAIL;
	}

		for (auto& targetObject : *(pTargetLayer->Get_ObjectLayer()))
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
	

	return S_OK;
}

HRESULT CNaviMeshController::Compare_Navi_MeshMove(Engine::CLayer* pTargetLayer)
{
	if (m_pNaviMeshCom == nullptr)
	{
		return E_FAIL;
	}
	if (pTargetLayer == nullptr)
	{
		return S_OK;
	}

	for (auto& targetObject : *(pTargetLayer->Get_ObjectLayer()))
	{
		if (targetObject.second == nullptr)
		{
			continue;
		}
		CDynamicObject* pObject = dynamic_cast<CDynamicObject*>(targetObject.second);
		//스폰중일때는 검사 안함 
		if (pObject != nullptr && (pObject->Get_State() == true))
		{
			pObject->Set_Position(
				m_pNaviMeshCom->Compare_OnNaviMesh_for_Mesh(
					&(pObject->Get_Position()), 
					&(pObject->Calc_Position_At_Mesh()), 
					&(pObject->Get_Position_At_Mesh()))
			);
		}
	}


	return S_OK;
}

_bool CNaviMeshController::Stand_NaviMesh(Engine::CGameObject* pObject, float* pfCalPos)
{
	if (m_pNaviMeshCom == nullptr)
	{
		return false;
	}

	if (pObject == nullptr)
	{
		return false;
	}

	_vec3 vCalPos = m_pNaviMeshCom->Caculate_OnNaviMesh(&(pObject->Get_Position()));
	_float fY = pObject->Get_Position().y;

	if (vCalPos.y > pObject->Get_Position().y)
	{
		if (pfCalPos != nullptr)
		{
			*pfCalPos = vCalPos.y;
		}

		return true;
	}
	return false;
}

void CNaviMeshController::Clear_NaviMesh()
{
	m_pNaviMeshCom->Clear_NaviMesh();
}

void CNaviMeshController::Set_Position(_vec3 vPos)
{
	//안씀
}
void CNaviMeshController::Set_Size(_vec3 vSize)
{
	//안씀
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
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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

