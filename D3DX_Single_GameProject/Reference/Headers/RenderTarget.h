#pragma once

#ifndef __RENDER_TARGET_H__
#define __RENDER_TARGET_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CRenderTarget : public CBase
{
private:
	explicit CRenderTarget(_Device pDevice);
	virtual ~CRenderTarget() = default;

public:
	HRESULT			Ready_RenderTarget(const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);

	void			SetUp_OnGraphicDevice(const _uint& iIndex);
	void			Release_OnGraphicDevice(const _uint& iIndex);

	void			Clear_RenderTarget(void);

	HRESULT			Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void			Render_DebugBuffer(void);

	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const char* pConstantTable);

private:
	_Device m_pDevice;
	LPDIRECT3DTEXTURE9			m_pTargetTexture;

	LPDIRECT3DSURFACE9			m_pTargetSurface;
	LPDIRECT3DSURFACE9			m_pOldTargetSurface;

	D3DXCOLOR					m_ClearColor;

private:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;

public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);
	virtual void				Free(void);

};

END_NAMESPACE

#endif // !__RENDER_TARGET_H__