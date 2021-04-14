#include "Engine_Include.h"
#include "FontManager.h"
#include "Font.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontManager)

CFontManager::CFontManager()
{
}

HRESULT CFontManager::Ready_Font(_Device pDevice, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight, _bool bExtra)
{
	CFont* pFont = nullptr;

	pFont = Find_Font(pFontTag);
	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pDevice, pFontType, iWidth, iHeight, iWeight, bExtra);
	NULL_CHECK_RETURN(pFont, E_FAIL);

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}


void CFontManager::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFont* pFont = Find_Font(pFontTag);
	NULL_CHECK(pFont);



	pFont->Render_Font(pString, pPos, Color);
}

void CFontManager::Render_Font_Bottom(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, const _vec2* pSize, D3DXCOLOR Color)
{
	CFont* pFont = Find_Font(pFontTag);
	NULL_CHECK(pFont);



	pFont->Render_Font_Bottom(pString, pPos, pSize, Color);
}

CFont* CFontManager::Find_Font(const _tchar* pFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void CFontManager::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}
