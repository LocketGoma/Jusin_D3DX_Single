#include "Engine_Include.h"
#include "Font.h"

USING(Engine)

CFont::CFont(_Device pDevice)
	: m_pDevice(pDevice)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pDevice->AddRef();
}

HRESULT CFont::Ready_Font(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Width = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont)))
	{
		PRINT_LOG(L"Error",L"Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		PRINT_LOG(L"Error",L"Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CFont::Ready_Font_Extra(const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	D3DXFONT_DESC	Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = DEFAULT_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Width = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &Font_Desc, &m_pFont)))
	{
		PRINT_LOG(L"Error", L"Font Create Failed");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateSprite(m_pDevice, &m_pSprite)))
	{
		PRINT_LOG(L"Error", L"Sprite Create Failed");
		return E_FAIL;
	}

	return S_OK;
}

void CFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT	rc{ _long(pPos->x), _long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();
}

CFont* CFont::Create(_Device pDevice, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight, _bool bType)
{
	CFont* pInstance = new CFont(pDevice);

	if (bType == true)
	{
		if (FAILED(pInstance->Ready_Font_Extra(pFontType, iWidth, iHeight, iWeight)))
		{
			PRINT_LOG(L"Error", L"Extra Font Setting Failed");
			Safe_Release(pInstance);
		}
	}
	else
	{
		if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
		{
			PRINT_LOG(L"Error", L"Font Setting Failed");
			Safe_Release(pInstance);
		}
	}

	return pInstance;
}

void CFont::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pDevice);
}
