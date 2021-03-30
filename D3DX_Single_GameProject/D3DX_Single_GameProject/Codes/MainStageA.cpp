#include "framework.h"
#include "MainStageA.h"

#include "MainMapA.h"
#include "SkyBox.h"
#include "Light.h"
#include "LightingManager.h"

#include "NaviMeshController.h"

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

	return S_OK;
}

_int CMainStageA::Update_Scene(const _float& fDeltaTime)
{
	CScene::Update_Scene(fDeltaTime);

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

	pGameObject = pManagement->Clone_GameObject(L"MainCamera");
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->Add_GameObject(L"MainCamera", pGameObject);

	pLayer->Find_GameObject(L"Player");


	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;

}

HRESULT CMainStageA::Add_Enemy_Layer(const _tchar* pLayerTag)
{
	return E_NOTIMPL;
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
	return E_NOTIMPL;
}

CMainStageA* CMainStageA::Create(_Device pDevice)
{
	return nullptr;
}

void CMainStageA::Free()
{
}
