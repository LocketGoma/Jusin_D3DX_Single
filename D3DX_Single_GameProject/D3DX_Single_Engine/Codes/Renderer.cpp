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

//�� �����Ӹ��� �������� ��� �߰��ϴ°�... ���ո��ϰ� ���̱� �մϴٸ�
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
    //���� ����Ʈ �߰�

    if (FAILED(Render_Priority()))
        return E_FAIL;

    if (FAILED(Render_Priority_Alpha()))
        return E_FAIL;

    if (FAILED(Render_NoAlpha()))
        return E_FAIL;

    if (FAILED(Render_Alpha()))
        return E_FAIL;

    if (FAILED(Render_HalfAlpha()))
        return E_FAIL; 
    
    if (FAILED(Render_Wireframe()))
        return E_FAIL;

    if (FAILED(Render_UI()))
        return E_FAIL;

    //���� ����Ʈ ��
   
    if (FAILED(CGraphicDevice::Get_Instance()->Render_End(hWND)))
    {
        PRINT_LOG(L"Render Error", L"Failed to Render_End (maybe occur m_pDevice Failed)");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT CRenderer::Render_Priority()
{
    //m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY])
    {
        if (FAILED(pGameObject->Render_GameObject()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Priority_Alpha()
{

    if (m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY_ALPHA].empty() == true)
    {
        return S_OK;
    }


    ////���ĺ��� ����

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
	{
		return E_FAIL;
	}
	//���ĺ��� �ɼ� ����
	if (FAILED(m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD)))
	{
		return E_FAIL;
	}
	//Source�ȼ�(=�׷��� �� �ȼ�) �� ȥ�� ���� ����
	if (FAILED(m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
	{
		return E_FAIL;
	}
	//Dest�ȼ��� ȥ�� ���� ����
    //D3DRS_DESTBLEND: Dest�ȼ�(�̹� �׷����ִ� �ȼ�)
    //D3DBLEND_INVSRCALPHA: ȥ�պ����̸� 1 - D3DBLEND_SRCALPHA. 0~1����.
    //ex) D3DBLEND_SRCALPHA(0.7f, 0.7f, 0.7f, 0.7f) -> D3DBLEND_INVSRCALPHA(0.3f, 0.3f, 0.3f, 0.3f)
 
	if (FAILED(m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA)))
	{
		return E_FAIL;
	}

	for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY_ALPHA])
	{
		if (FAILED(pGameObject->Render_GameObject()))
			return E_FAIL;

		Safe_Release(pGameObject);
	}

	m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY_ALPHA].clear();


	if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
	{
		return E_FAIL;
	}


    return S_OK;
}

HRESULT CRenderer::Render_NoAlpha()
{
    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_NOALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_NOALPHA].clear();

    return S_OK;
}


HRESULT CRenderer::Render_HalfAlpha()
{
    if (m_GameObjects[(_uint)RENDERID::RENDER_PRIORITY_ALPHA].empty() == true)
    {
        return S_OK;
    }

    ////���ĺ��� ����
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
    {
        return E_FAIL;
    }
    //���ĺ��� �ɼ� ����
    if (FAILED(m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD)))
    {
        return E_FAIL;
    }
    //Source�ȼ�(=�׷��� �� �ȼ�) �� ȥ�� ���� ����
    if (FAILED(m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
    {
        return E_FAIL;
    }

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_HALFALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_HALFALPHA].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
    //��� �����׽���

    if (m_GameObjects[(_uint)RENDERID::RENDER_ALPHA].empty())
    {
        return S_OK;
    }
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 128)))         //���İ� ������ �� �ٲٸ� ��
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
    {
        return E_FAIL;
    }

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_HALFALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_HALFALPHA].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Render_Wireframe()
{
    //���� ���� ó�����ϱ� ������Ʈ �ϳ��� ������ ����

    if (m_GameObjects[(_uint)RENDERID::RENDER_WIREFRAME].empty())
    {
        return S_OK;
    }
    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
    {
        return E_FAIL;
    }

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_WIREFRAME])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_WIREFRAME].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Render_UI()
{
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 128)))         //���İ� ������ �� �ٲٸ� ��
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
    {
        return E_FAIL;
    }

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_UI])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_UI].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE)))
    {
        return E_FAIL;
    }




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

        if (!m_GameObjects[i].empty())
        {
            return E_FAIL;
        }
    }

    return S_OK;
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
