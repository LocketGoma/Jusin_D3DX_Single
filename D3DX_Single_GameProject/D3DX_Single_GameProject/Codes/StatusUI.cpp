#include "framework.h"
#include "StatusUI.h"

#include "VTXRectTexture.h"
#include "Transform.h"
#include "Texture.h"

#include "Player.h"

CStatusUI::CStatusUI(_Device pDevice)
	: CBaseEffect(pDevice)
	, m_iHP(0)
	, m_iSuitHP(0)
	, m_iMagAmmo(0)
	, m_iAmmo(0)
	, m_iAltAmmo(-1)
{
	ZeroMemory(m_bStatus, sizeof(bool) * (_uint)eStatusType::END);
}

CStatusUI::CStatusUI(const CStatusUI& other)
	: CBaseEffect(other)
	, m_iHP(other.m_iHP)
	, m_iSuitHP(other.m_iSuitHP)
	, m_iMagAmmo(other.m_iMagAmmo)
	, m_iAmmo(other.m_iAmmo)
	, m_iAltAmmo(other.m_iAltAmmo)
{
	ZeroMemory(m_bStatus, sizeof(bool) * (_uint)eStatusType::END);
	m_bStatus[(_uint)eStatusType::HP] = true;
	m_bStatus[(_uint)eStatusType::SUIT] = true;
	m_bStatus[(_uint)eStatusType::AMMO] = true;

}

HRESULT CStatusUI::Ready_GameObject(void)
{
	return S_OK;
}

HRESULT CStatusUI::Ready_GameObject_Clone(void* pArg)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CStatusUI::Update_GameObject(const _float& fDeltaTime)
{
	return NO_EVENT;
}

_int CStatusUI::LateUpdate_GameObject(const _float& fDeltaTime)
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	pManagement->Add_RenderList(Engine::RENDERID::RENDER_UI_ALPHA, this);


	return NO_EVENT;
}

HRESULT CStatusUI::Render_GameObject(void)
{

	Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
	if (pManagement == nullptr)
	{
		return E_FAIL;
	}

	Change_To_Ortho();

	if (FAILED(CGameObject::Render_GameObject()))
	{
		return E_FAIL;
	}


	if (FAILED(m_pTextureCom->Set_Texture()))
	{
		return E_FAIL;
	}


	//탄약
	if (m_bStatus[(_uint)eStatusType::AMMO] == true)
	{
		Set_EffectPosition(_vec3(360.f, 140.f, 0.f), _vec3((WINCX >> 2) + (WINCX >> 4), -((WINCY >> 2) + (WINCY >> 3)), 0.f));
		m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
		m_pDevice->SetTransform(D3DTS_VIEW, &matView);
		m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);


		wsprintf(m_szAmmo, L"%d", m_iMagAmmo);
		pManagement->Render_Font(L"Font_Status_Gauge", m_szAmmo, &_vec2((WINCX >> 1) + (WINCX >> 2), WINCY - (WINCY >> 2) + (WINCY >> 4)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));

		wsprintf(m_szAmmo, L"%d", m_iAmmo);
		pManagement->Render_Font(L"Font_Status_Gauge_Mini", m_szAmmo, &_vec2((WINCX >> 1) + (WINCX >> 2)+(WINCX>>4), WINCY - (WINCY >> 2) + (WINCY>>5) + (WINCY >> 4)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));

		pManagement->Render_Font(L"Font_Status_MSG", m_szAmmoMsg, &_vec2((WINCX >> 1) + (WINCX >> 2), WINCY - (WINCY >> 3) + (WINCY >> 5)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));


		if (FAILED(m_pBufferCom->Render_Buffer()))
		{
			return E_FAIL;
		}
	}


	//체력
	Set_EffectPosition(_vec3(180.f, 120.f, 0.f), _vec3(-(WINCX >> 1) + (WINCX >> 3), -((WINCY >> 2) + (WINCY >> 3)), 0.f));
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);



	wsprintf(m_szHP, L"%d", m_iHP);
	pManagement->Render_Font(L"Font_Status_Gauge", m_szHP, &_vec2((WINCX >> 4)+(WINCX>>5), WINCY - (WINCY >> 2) + (WINCY >> 4)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));
	pManagement->Render_Font(L"Font_Status_MSG", m_szHPmsg, &_vec2((WINCX >> 4) + (WINCX >> 5), WINCY - (WINCY >> 3) + (WINCY >> 5)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));
	if (FAILED(m_pBufferCom_HP->Render_Buffer()))
	{
		return E_FAIL;
	}



	//슈트
	Set_EffectPosition(_vec3(180.f, 120.f, 0.f), _vec3(-(WINCX >> 1) + (WINCX >> 2) + (WINCX >> 4), -((WINCY >> 2) + (WINCY >> 3)), 0.f));
	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);


	wsprintf(m_szSuitHP, L"%d", m_iSuitHP);
	pManagement->Render_Font(L"Font_Status_Gauge", m_szSuitHP, &_vec2((WINCX >> 2) + (WINCX >> 5), WINCY - (WINCY >> 2)+ (WINCY >> 4)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));
	pManagement->Render_Font(L"Font_Status_MSG", m_szSuitMsg, &_vec2((WINCX >> 2) + (WINCX >> 5), WINCY - (WINCY >> 3) + (WINCY >> 5)), D3DXCOLOR(1.0f, 1.0f, 0.3f, 1.0f));

	if (FAILED(m_pBufferCom_SUIT->Render_Buffer()))
	{
		return E_FAIL;
	}


	Change_To_Proj();

	return S_OK;
}

void CStatusUI::Set_Ortho(_vec3 vScale, _vec3 vPos)
{
	_mat matWorld, matView, matProj, matOrtho;


	m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matProj);

	D3DXMatrixOrthoLH(
		&matOrtho, /* 직교투영행렬 반환 */
		WINCX, // 가로폭 (WINCX)
		WINCY, // 세로폭 (WINCY)
		0, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
		1 /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);

	matProj = matOrtho;


	matView._11 = vScale.x;
	matView._22 = vScale.y;
	matView._33 = vScale.z;

	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void CStatusUI::Set_EffectPosition(_vec3 vScale, _vec3 vPos)
{
	m_vScale = vScale;
	m_vPos = vPos;

	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&matOrtho);

	D3DXMatrixOrthoLH(
		&matOrtho, /* 직교투영행렬 반환 */
		WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
		WINCY, // 세로폭 (WINCY)
		0, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
		1 /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);


	matView._11 = vScale.x;
	matView._22 = vScale.y;
	matView._33 = vScale.z;

	matView._41 = vPos.x;
	matView._42 = vPos.y;
	matView._43 = vPos.z;

}

void CStatusUI::Set_EffectVisualble(eStatusType eType, bool bState)
{
	m_bStatus[(_uint)eType] = bState;
}

void CStatusUI::Set_HP(_uint iHP)
{
	m_iHP = iHP;
}

void CStatusUI::Set_SuitHP(_uint iSuitHP)
{
	m_iSuitHP = iSuitHP;
}

void CStatusUI::Set_MagAmmo(_uint iMagAmmo)
{
	m_iMagAmmo = iMagAmmo;
}

void CStatusUI::Set_Ammo(_uint iAmmo)
{
	m_iAmmo = iAmmo;
}

void CStatusUI::Set_AltAmmo(int iAltAmmo)
{
	m_iAltAmmo = iAltAmmo;
}

void CStatusUI::Change_To_Ortho()
{
	m_pDevice->GetTransform(D3DTS_VIEW, &matOriView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOriProj);

	m_pDevice->SetTransform(D3DTS_WORLD, &matWorld);
	m_pDevice->SetTransform(D3DTS_VIEW, &matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
}

void CStatusUI::Change_To_Proj()
{
	m_pDevice->SetTransform(D3DTS_VIEW, &matOriView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOriProj);
}

HRESULT CStatusUI::Add_Component()
{
	auto pManagement = Engine::CManagement::Get_Instance();
	if (nullptr == pManagement)
	{
		return MANAGER_OUT;
	}

	Engine::CComponent* pComponent = nullptr;

	// Buffer
	pComponent = m_pBufferCom = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer_Ammo", pComponent);
	pComponent = m_pBufferCom_HP = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer_HP", pComponent);
	pComponent = m_pBufferCom_SUIT = dynamic_cast<Engine::CVTXRectTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_BUFFER, L"Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Buffer_SUIT", pComponent);

	//트랜스폼
	pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// Texture
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>(pManagement->Clone_Resource((_uint)RESOURCETYPE::RESOURCE_TEXTURE, L"Texture_UI_BOX"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[(_uint)Engine::COMPONENT_ID::ID_STATIC].emplace(L"Com_Texture", pComponent);

	return S_OK;
}

void CStatusUI::Free()
{
	Safe_Release(m_pBufferCom_HP);
	Safe_Release(m_pBufferCom_SUIT);

	CBaseEffect::Free();

}

CStatusUI* CStatusUI::Create(_Device pDevice)
{
	CStatusUI* pInstance = new CStatusUI(pDevice);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

Engine::CGameObject* CStatusUI::Clone(void* pArg)
{
	CStatusUI* pClone = new CStatusUI(*this);

	if (pClone == nullptr)
	{
		PRINT_LOG(L"Error", L"Failed To Clone CStatusUI");

		return nullptr;
	}
	if (FAILED(pClone->Ready_GameObject_Clone()))
	{
		Safe_Release(pClone);
	}


	return pClone;
}
