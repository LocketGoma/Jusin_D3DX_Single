#include "framework.h"
#include "MainStageC.h"

#include "MainMapC.h"
#include "SkyBox.h"
#include "Light.h"
#include "LightingManager.h"

#include "NaviMeshController.h"

#include "Player.h"
#include "BaseAI.h"
#include "BaseAI_Attacker.h"
#include "DynamicObject.h"
#include "BaseProjectile.h"

CMainStageC::CMainStageC(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pNaviController(nullptr)
{
}

HRESULT CMainStageC::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);

	SetWindowText(g_hWnd, L"Stage C");

	Add_Player_Layer(L"PlayerLayer");
	Add_Boss_Layer(L"BossLayer");
	Add_Object_Layer(L"ObjectLayer");
	Add_Enemy_Layer(L"EnemyLayer");
	Add_Environment_Layer(L"EnviromentLayer");
	Add_Weapon_Layer(L"WeaponLayer");

	return S_OK;
}

_int CMainStageC::Update_Scene(const _float& fDeltaTime)
{
	return _int();
}

_int CMainStageC::LateUpdate_Scene(const _float& fDeltaTime)
{
	return _int();
}

void CMainStageC::Render_Scene(void)
{
}

HRESULT CMainStageC::Ready_Resource(_Device& pDevice)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Player_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Enemy_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Boss_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Object_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Environment_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

HRESULT CMainStageC::Add_Weapon_Layer(const _tchar* pLayerTag)
{
	return S_OK;
}

CMainStageC* CMainStageC::Create(_Device pDevice)
{
	CMainStageC* pInstance = new CMainStageC(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainStageC::Free()
{
	Engine::CScene::Free();
}
