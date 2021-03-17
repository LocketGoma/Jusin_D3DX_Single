#pragma once
#ifndef __FONT_H__
#define __FONT_H__

#include "Engine_Define.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CFont : public CBase
{
private:
	explicit CFont(_Device pDevice);
	virtual ~CFont(void) = default;

public:
	HRESULT		Ready_Font(const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void		Render_Font(const _tchar* pString,
		const _vec2* pPos,
		D3DXCOLOR Color);

private:
	_Device					m_pDevice;
	LPD3DXFONT				m_pFont;
	LPD3DXSPRITE			m_pSprite;

public:
	static CFont* Create(_Device pDevice,
		const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

private:
	virtual void Free(void);

};

END_NAMESPACE
#endif // __FONT_H__
