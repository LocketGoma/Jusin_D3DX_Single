#include "Engine_Include.h"
#include "Renderer.h"
#include "GraphicDevice.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer(/*LPDIRECT3DDEVICE9 pDevice*/)
//: m_pDevice(pDevice)
{
    //Safe_AddRef(m_pDevice);
}

HRESULT CRenderer::Set_Device(_Device pDevice, LPD3DXSPRITE pSprite, LPD3DXFONT pFont)
{
    if (pDevice == nullptr)
    {
        return E_FAIL;
    }

    m_pDevice = pDevice;
    Safe_AddReference(m_pDevice);

    if (pSprite != nullptr)
    {
        m_pSprite = pSprite;
        Safe_AddReference(m_pSprite);
    }
    if (pFont != nullptr) 
    {
        m_pFont = pFont;
        Safe_AddReference(m_pFont);
    }

    return S_OK;
}

//매 프레임마다 랜더러에 대상 추가하는게... 불합리하게 보이긴 합니다만
HRESULT CRenderer::Add_RenderList(RENDERID eRenderID, CGameObject* pGameObject)
{
    if ((_uint)RENDERID::RENDER_END <= (_uint)eRenderID ||
        pGameObject == nullptr)
        return E_FAIL;

    m_GameObjects[(_uint)eRenderID].push_back(pGameObject);
    Safe_AddReference(pGameObject);

    return S_OK;
}

HRESULT CRenderer::Render_RenderList(HWND hWND)
{
    if (m_pDevice == nullptr)
    {
        return E_FAIL;
    }    
    if (FAILED(CGraphicDevice::Get_Instance()->Render_Begin(D3DCOLOR_ARGB(255, 100, 100, 100))))
    {
        PRINT_LOG(L"Render Error", L"Failed to Render_Begin (maybe occur m_pDevice Failed)");
        return E_FAIL;
    }
    //랜더 리스트 추가

    if (FAILED(Render_Priority()))
        return E_FAIL;

    if (FAILED(Render_NoAlpha()))
        return E_FAIL;

    if (FAILED(Render_Alpha()))
        return E_FAIL;

    if (FAILED(Render_HalfAlpha()))
        return E_FAIL;

    if (FAILED(Render_UI()))
        return E_FAIL;

    //랜더 리스트 끝
   
    if (FAILED(CGraphicDevice::Get_Instance()->Render_End(hWND)))
    {
        PRINT_LOG(L"Render Error", L"Failed to Render_End (maybe occur m_pDevice Failed)");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CRenderer::Render_Priority()
{
    return S_OK;
}

HRESULT CRenderer::Render_NoAlpha()
{
    return S_OK;
}

HRESULT CRenderer::Render_Priority_Alpha()
{
    return S_OK;
}

HRESULT CRenderer::Render_HalfAlpha()
{
    return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
    return S_OK;
}

HRESULT CRenderer::Render_Wireframe()
{
    return S_OK;
}

HRESULT CRenderer::Render_UI()
{
    return S_OK;
}

HRESULT CRenderer::Cleaer_RenderList()
{
    for (_uint i = 0; i < (_uint)RENDERID::RENDER_END; ++i)
    {
        for (auto& pGameObject : m_GameObjects[i])
        {
            Safe_Release(pGameObject);
        }

        m_GameObjects[i].clear();
    }
}

void CRenderer::Free()
{
    for (_uint i = 0; i < (_uint)RENDERID::RENDER_END; ++i)
    {
        for (auto& pGameObject : m_GameObjects[i])
        {
            Safe_Release(pGameObject);
        }

        m_GameObjects[i].clear();
    }

    Safe_Release(m_pDevice);

}
