#include "framework.h"
#include "BaseLogoScene.h"

CBaseLogoScene::CBaseLogoScene(_Device pDevice)
	: Engine::CScene(pDevice)
	, m_pLoading(nullptr)
{
}

HRESULT CBaseLogoScene::Ready_Scene(void)
{
	return E_NOTIMPL;
}

_int CBaseLogoScene::Update_Scene(const _float& fTimeDelta)
{
	return _int();
}

_int CBaseLogoScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	return _int();
}

void CBaseLogoScene::Render_Scene(void)
{
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

	//화면 띄우기용 텍스쳐 로드



	return S_OK;
}

unsigned int __stdcall CBaseLogoScene::Thread_Load(void* pArg)
{
	return 0;
}

CBaseLogoScene* CBaseLogoScene::Create(_Device pDevice, LOADINGID eLoadingID)
{
	return nullptr;
}
