#include "framework.h"
#include "CrossHairUI.h"

CCrossHairUI::CCrossHairUI(_Device pDevice)
	: CBaseEffect(pDevice)
	, m_iHP(100)
	, m_iMagAmmo(1)
	, m_iMagFullAmmo(1)
{
}

CCrossHairUI::CCrossHairUI(const CCrossHairUI& other)
	: CBaseEffect(other)
	, m_iHP(100)
	, m_iMagAmmo(1)
	, m_iMagFullAmmo(1)
{
}

HRESULT CCrossHairUI::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CCrossHairUI::Ready_GameObject_Clone(void* pArg)
{
	return S_OK;
}

_int CCrossHairUI::Update_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}

_int CCrossHairUI::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI_ALPHA, this);

	return NO_EVENT;
}

HRESULT CCrossHairUI::Render_GameObject(void)
{

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}
	if (FAILED(CGameObject::Render_GameObject()))
	{
		return E_FAIL;
	}

	D3DXCOLOR pColor = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.8f);
	D3DXCOLOR pAmmoColor = D3DXCOLOR(1.0f, 1.0f, 0.3f, 0.8f);
	D3DXCOLOR pRedColor = D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f);
	if (m_iHP < 45)
	{
		pColor = pRedColor;
	}
	if (((float)m_iMagAmmo / (float)m_iMagFullAmmo) < 0.45f)
	{
		pAmmoColor = pRedColor;
	}
	//연립방정식
	//20 = Ax0 +b -> b = 20
	//95 = Ax100 + b -> Ax100 + 20 => Ax100 = 75 => A = 0.75


	pManagement->Render_Font_Bottom(L"Font_UI_Cross", m_szLeft_HP_Empty, &_vec2((WINCX >> 1) - 60, (WINCY >> 1) + 55), &_vec2(150, 100), pColor);
	pManagement->Render_Font_Bottom(L"Font_UI_Cross", m_szLeft_HP_Full, &_vec2((WINCX >> 1) - 60, (WINCY >> 1) + 55), &_vec2(150.f, 100.f * (((float)m_iHP / 100.f)*0.75f+0.2f)), pColor);
	

	pManagement->Render_Font_Bottom(L"Font_UI_Cross", m_szRight_Ammo_Empty, &_vec2((WINCX >> 1) + 30, (WINCY >> 1) + 55), &_vec2(150, 100), pAmmoColor);
	pManagement->Render_Font_Bottom(L"Font_UI_Cross", m_szRight_Ammo_Full, &_vec2((WINCX >> 1) + 30, (WINCY >> 1)+55), &_vec2(150, 100.f * (((float)m_iMagAmmo / (float)m_iMagFullAmmo) * 0.75f + 0.2f)), pAmmoColor);

	
	pManagement->Render_Font(L"Font_UI_Cross_Core", m_szCenter_CrossHair, &_vec2((WINCX >> 1)-30, (WINCY >> 1)-50), D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f));


	return S_OK;
}

void CCrossHairUI::Set_HP(_uint iHP)
{
	m_iHP = iHP;
}


void CCrossHairUI::Set_MagAmmo(_int iMagAmmo)
{
	if (iMagAmmo < 0)
	{
		m_iMagAmmo = 1;
	}
	else
	{
		m_iMagAmmo = iMagAmmo;
	}
}

void CCrossHairUI::Set_MagFullAmmo(_int iFullAmmo)
{
	if (iFullAmmo < 0)
	{
		m_iMagFullAmmo = 1;
	}
	else
	{
		m_iMagFullAmmo = iFullAmmo;
	}
}


void CCrossHairUI::Free()
{
	CBaseEffect::Free();
}

CCrossHairUI* CCrossHairUI::Create(_Device pDevice)
{
	CCrossHairUI* pInstance = new CCrossHairUI(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CCrossHairUI::Clone(void* pArg)
{
	CCrossHairUI* pClone = new CCrossHairUI(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStatusUI");

		return nullptr;
	}

	return pClone;
}
