#pragma once

#ifndef __GRAPHIC_DEVICE_H__
#define __GRAPHIC_DEVICE_H__

#include "Engine_Include.h"
#include "Base.h"

BEGIN_NAMESPACE(Engine)

class CGraphicDevice : public CBase
{
	DECLARE_SINGLETON(CGraphicDevice)

private :
	explicit CGraphicDevice();
	virtual ~CGraphicDevice();

public:
	_Device GetDevice() { return m_pDevice; }

public:
	HRESULT Ready_GraphicDevice(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY, CGraphicDevice** ppDevice);
	HRESULT Render_Begin(D3DXCOLOR Color);
	HRESULT Render_End(HWND hWnd = nullptr);


private:
	LPDIRECT3D9				m_pSDK = nullptr;
	LPDIRECT3DDEVICE9		m_pDevice = nullptr;

public :
	virtual void Free();
};

END_NAMESPACE

#endif // !__GRAPHIC_DEVICE_H__