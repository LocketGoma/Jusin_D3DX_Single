#include "framework.h"
#include "BaseLogoScene.h"
#include "LogoImage.h"

#include "TestStage.h"
#include "MainStageA.h"
#include "MainStageB.h"
#include "MainStageC.h"
#include "LoadingScene.h"
#include "Transform.h"

#include "Management.h"

CBaseLogoScene::CBaseLogoScene(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pLoading(nullptr)
{
}

HRESULT CBaseLogoScene::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Resource(m_pDevice), E_FAIL);


	//임시 카메라 설정
	_mat m_matView, m_matProj;
	
	D3DXMatrixLookAtLH(&m_matView, &_vec3(0.f, 0.f, -5.f), &_vec3(0.f, 0.f, 1.f), &_vec3(0.f, 1.f, 0.f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(60.f), _float(WINCX) / WINCY, 0.1f, 1000.f);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pLoading = CLoadingScene::Create(m_pDevice, LOADINGID::LOADING_TEST);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	Ready_Layer(L"LogoImage");

	return S_OK;
}

_int CBaseLogoScene::Update_Scene(const _float& fDeltaTime)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	_int iExit = Engine::CScene::Update_Scene(fDeltaTime);

	if (m_pLoading->IsFinished() == true)
	{
		if (pManagement->Key_Down(VK_RETURN))
		{
			pManagement->Stop_AllSound();

			ShowCursor(false);

			m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			Safe_Release(m_pLoading);

			//pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_TEST, CTestStage::Create(m_pDevice));
			pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_STAGE1, CMainStageA::Create(m_pDevice));
			//pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_STAGE2, CMainStageB::Create(m_pDevice));
			//pManagement->Setup_CurrentScene((_uint)ESceneType::SCENE_STAGE3, CMainStageC::Create(m_pDevice));

			return iExit;
		}
	}
	return _int();
}

_int CBaseLogoScene::LateUpdate_Scene(const _float& fDeltaTime)
{
	CScene::LateUpdate_Scene(fDeltaTime);


	return _int();
}

void CBaseLogoScene::Render_Scene(void)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return;
	}
	pManagement->Render_Font(L"Font_BASE", m_pLoading->Get_String(), &_vec2((WINCX>>1) + (WINCX>>2), WINCY-20), D3DXCOLOR(1.0f, 1.0f, 1.f, 1.0f));	
}
//Resource Loader
HRESULT CBaseLogoScene::Ready_Resource(_Device pDevice)
{
	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	pManagement->Reserve_Size((_uint)RESOURCETYPE::RESOURCE_END);

	//화면 띄우기용 버퍼 로드
	pManagement->Ready_Buffer(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex", Engine::BUFFERID::BUFFER_RCTEX);

	//화면 띄우기용 텍스쳐 로드
	pManagement->Ready_Texture(m_pDevice, (_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_Logo", Engine::TEXTYPE::TEX_NORMAL, L"../../Resource/Image/Loading%d.jpg", 1);

	//화면 띄우기용 트랜스폼 로드
	pManagement->Ready_Prototype(L"Transform_Comp", Engine::CTransform::Create(m_pDevice));


	return S_OK;
}

HRESULT CBaseLogoScene::Ready_Layer(const _tchar* pLayerTag)
{
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	Engine::CLayer* pLayer = Engine::CLayer::Create();

	Engine::CGameObject* pGameObject = nullptr;

	pGameObject = CLogoImage::Create(m_pDevice);
	pLayer->Add_GameObject(L"LogoImage", pGameObject);

	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

CBaseLogoScene* CBaseLogoScene::Create(_Device pDevice)
{
	CBaseLogoScene* pInstance = new CBaseLogoScene(pDevice);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBaseLogoScene::Free()
{
	Engine::CScene::Free();
}
