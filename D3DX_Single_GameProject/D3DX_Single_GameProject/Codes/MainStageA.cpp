#include "framework.h"
#include "MainStageA.h"

#include "MainMapA.h"
#include "SkyBox.h"
#include "Light.h"
#include "LightingManager.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "BaseAI.h"
#include "BaseAI_Attacker.h"
#include "DynamicObject.h"
#include "BaseProjectile.h"

CMainStageA::CMainStageA(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pNaviController(nullptr)
{
}

HRESULT CMainStageA::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

	SetWindowText(g_hWnd, L"Stage A");

	Add_Player_Layer(L"PlayerLayer");
	Add_Boss_Layer(L"BossLayer");
	Add_Object_Layer(L"ObjectLayer");
	Add_Enemy_Layer(L"EnemyLayer");
	Add_Environment_Layer(L"EnviromentLayer");
	Add_Weapon_Layer(L"WeaponLayer");

	return S_OK;
}

_int CMainStageA::Update_Scene(const _float& fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

	m_pNaviController->Compare_NaviMove(Get_Layer(L"PlayerLayer"));
	m_pNaviController->Compare_Navi_MeshMove(Get_Layer(L"EnemyLayer"));

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(Get_Layer(L"PlayerLayer")->Get_GameObject(L"Player"));
	if (pPlayer == nullptr)
	{
		return E_FAIL;
	}

	if (m_pNaviController->Stand_NaviMesh(pPlayer))
	{
		pPlayer->Jump_Cancel();
	}


	return NO_EVENT;
}

_int CMainStageA::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);

	return NO_EVENT;
}

void CMainStageA::Render_Scene(void)
{
	//CScene::Render_Scene();

}

HRESULT CMainStageA::Ready_Resource(_Device& pDevice)
{
	return S_OK;
}

HRESULT CMainStageA::Add_Player_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	auto pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	pGameObject = pManagement->Clone_GameObject(L"Player");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"Player", pGameObject);
	pGameObject->Set_Position(_vec3(155.f, 14.f, -240.f));

	pGameObject = pManagement->Clone_GameObject(L"PlayerCamera");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"PlayerCamera", pGameObject);


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;

}

HRESULT CMainStageA::Add_Enemy_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();
	Engine::CLayer* pAILayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	auto pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	///적 스폰 파트
	//개미귀신
	//pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->Set_Position(_vec3(10.f, 0.f, 15.f));
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion1", pGameObject), E_FAIL);
	
	//pGameObject = pManagement->Clone_GameObject(L"EnemyAntlion");
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//pGameObject->Set_Position(_vec3(16.f, 0.f, 15.f));
	//FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Antlion2", pGameObject), E_FAIL);

	///기본 적 AI파트
	//개미귀신
	//pGameObject = CBaseAI_Attacker::Create(m_pDevice);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion1")));
	//dynamic_cast<CBaseAI_Attacker*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	//FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion1AI", pGameObject), E_FAIL);

	//pGameObject = CBaseAI::Create(m_pDevice);
	//NULL_CHECK_RETURN(pGameObject, E_FAIL);
	//dynamic_cast<CBaseAI*>(pGameObject)->Set_ControlUnit(dynamic_cast<CDynamicObject*>(pLayer->Get_GameObject(L"Antlion2")));
	//dynamic_cast<CBaseAI*>(pGameObject)->Set_Target(m_mapLayer.find(L"PlayerLayer")->second->Find_GameObject(L"Player"));
	//FAILED_CHECK_RETURN(pAILayer->Add_GameObject(L"Antlion2AI", pGameObject), E_FAIL);

	m_mapLayer.emplace(pLayerTag, pLayer);
	m_mapLayer.emplace(L"AILayer", pAILayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Boss_Layer(const _tchar* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CMainStageA::Add_Object_Layer(const _tchar* pLayerTag)
{
	return E_NOTIMPL;
}

HRESULT CMainStageA::Add_Environment_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	auto pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}


	pGameObject = pManagement->Clone_GameObject(L"SkyBoxA");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"SkyBoxA", pGameObject);

	pGameObject = pManagement->Clone_GameObject(L"MapA");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MapA", pGameObject), E_FAIL);

	pGameObject = m_pNaviController = CNaviMeshController::Create(m_pDevice);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NaviMesh", pGameObject), E_FAIL);
	
	m_pNaviController->Set_NaviMesh_From_File(L"../../Resource/Meshes/Navi/map1.json");

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CMainStageA::Add_Weapon_Layer(const _tchar* pLayerTag)
{
	Engine::CLayer* pLayer = Engine::CLayer::Create();

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

CMainStageA* CMainStageA::Create(_Device pDevice)
{
	CMainStageA* pInstance = new CMainStageA(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainStageA::Free()
{
	Engine::CScene::Free();
}
