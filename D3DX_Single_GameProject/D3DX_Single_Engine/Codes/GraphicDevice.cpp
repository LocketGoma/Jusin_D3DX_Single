#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{    
}

//CGraphicDevice::~CGraphicDevice()
//{
//}

HRESULT CGraphicDevice::Ready_GraphicDevice(HWND hWnd, WINMODE eMode, const _uint& iSizeX, const _uint& iSizeY/*, CGraphicDevice** ppDevice*/)
{
    m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

    if (m_pSDK == nullptr)
    {
        return E_FAIL;
    }

    D3DCAPS9			DeviceCaps;
    ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

    if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
    {
        return E_FAIL;
    }

    _ulong		uFlag = 0;

    //하드웨어 버텍스 프로세싱이 되는가 / 안되는가
    if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
    {
        uFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
    }
    else
    {
        uFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
    }

    D3DPRESENT_PARAMETERS			Present_Parameter;
    ZeroMemory(&Present_Parameter, sizeof(D3DPRESENT_PARAMETERS));

    Present_Parameter.BackBufferWidth = iSizeX;
    Present_Parameter.BackBufferHeight = iSizeY;
    Present_Parameter.BackBufferFormat = D3DFMT_A8R8G8B8;
    Present_Parameter.BackBufferCount = 1;

    Present_Parameter.MultiSampleType = D3DMULTISAMPLE_NONE;
    Present_Parameter.MultiSampleQuality = 0;

    Present_Parameter.SwapEffect = D3DSWAPEFFECT_DISCARD;
    Present_Parameter.hDeviceWindow = hWnd;

    Present_Parameter.Windowed = (_uint)eMode;
    Present_Parameter.EnableAutoDepthStencil = TRUE;
    Present_Parameter.AutoDepthStencilFormat = D3DFMT_D24S8;

    Present_Parameter.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    Present_Parameter.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, uFlag, &Present_Parameter, &m_pDevice)))
    {
        return E_FAIL;
    }

    //*ppDevice = this;

    return S_OK;
}

HRESULT CGraphicDevice::Render_Begin(D3DXCOLOR Color)
{
    if (m_pDevice == nullptr)
    {
        return E_FAIL;
    }

    m_pDevice->Clear(0, nullptr,
        D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        Color, 1.f, 0);
    m_pDevice->BeginScene();

    return S_OK;
}

HRESULT CGraphicDevice::Render_End(HWND hWnd)
{
    if (m_pDevice == nullptr)
    {
        return E_FAIL;
    }

    m_pDevice->EndScene();

    //이거 3번째를 hwnd로 바꿀일도 있었던거로 기억하는데
    m_pDevice->Present(NULL, NULL, hWnd, NULL);

    return S_OK;
}

void CGraphicDevice::Free()
{
    _uint dwRefCnt = 0;

    if (dwRefCnt = Safe_Release(m_pDevice))
    {
        TCHAR msg[256] = L"";
        TCHAR Omsg[] = L"m_pGraphicDev Release Failed : %d";

        swprintf_s(msg, Omsg, dwRefCnt);
        PRINT_LOG(L"Fatal Error", msg);
    }


    if (dwRefCnt = Safe_Release(m_pSDK))
    {
        PRINT_LOG(L"Fatal Error", L"m_pSDK Release Failed");
    }

}
