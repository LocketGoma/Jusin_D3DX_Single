#pragma once

#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"


BEGIN_NAMESPACE(Engine)

class CFont;

class ENGINE_DLL CFontManager : public CBase
{
	DECLARE_SINGLETON(CFontManager);

private:
	explicit CFontManager();
	virtual ~CFontManager() = default;

public:
	HRESULT	Ready_Font(_Device pDevice,	const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);

	void	Render_Font(const _tchar* pFontTag,	const _tchar* pString, const _vec2* pPos,	D3DXCOLOR Color);

private:
	CFont* Find_Font(const _tchar* pFontTag);

private:
	std::map<const _tchar*, CFont*>	m_mapFont;

public:
	virtual void Free(void);
	

};

END_NAMESPACE

#endif // !__FONT_MANAGER_H__