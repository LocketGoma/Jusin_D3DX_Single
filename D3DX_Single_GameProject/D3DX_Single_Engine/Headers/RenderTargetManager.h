#pragma once

#ifndef __RENDER_TARGET_MANAGER_H__
#define __RENDER_TARGET_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"
#include "RenderTarget.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CRenderTargetManager : public CBase
{
	DECLARE_SINGLETON(CRenderTargetManager)

private:
	explicit CRenderTargetManager();
	virtual ~CRenderTargetManager() = default;

public:
	HRESULT		Ready_RenderTarget(const _tchar* pTargetTag, _Device pDevice, const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color);

	HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT		Begin_MRT(const _tchar* pMRTTag);
	HRESULT		End_MRT(const _tchar* pMRTTag);

	HRESULT			Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void			Render_DebugBuffer(const _tchar* pMRTTag);
	void			SetUp_OnShader(LPD3DXEFFECT& pEffect, const _tchar* pTargetTag, const char* pConstantTable);

private:
	CRenderTarget* Find_RenderTarget(const _tchar* pTargetTag);
	std::list<CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);

private:
	std::map<const _tchar*, CRenderTarget*>		m_mapRenderTarget;

	std::map<const _tchar*, std::list<CRenderTarget*>>	m_mapMRT;

public:
	virtual void Free(void);


};

END_NAMESPACE;

#endif // !__RENDER_TARGET_MANAGER_H__