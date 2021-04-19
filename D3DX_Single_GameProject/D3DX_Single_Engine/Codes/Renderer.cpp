#include "Engine_Include.h"
#include "Renderer.h"
#include "GraphicDevice.h"

#include "Management.h"

#include "Shader.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

CRenderer::CRenderer()
    : m_bVisuableDebug(true)
{

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

    //----랜더타겟용..........

    m_pManagement = CManagement::Get_Instance();    

    FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
        0, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
        FVF_SCREEN,
        D3DPOOL_MANAGED, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
        &m_pVB, NULL), E_FAIL);

    FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(sizeof(INDEX16) * 2,
        0,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &m_pIB, NULL), E_FAIL);

    D3DVIEWPORT9			ViewPort;
    m_pDevice->GetViewport(&ViewPort);

    VTXSCREEN* pVertex = NULL;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    pVertex[0].vPosition = _vec4(0.f, 0.f, 0.f, 1.f);
    pVertex[0].vTexUV = _vec2(0.f, 0.f);

    pVertex[1].vPosition = _vec4(_float(ViewPort.Width), 0.f, 0.f, 1.f);
    pVertex[1].vTexUV = _vec2(1.f, 0.f);

    pVertex[2].vPosition = _vec4(_float(ViewPort.Width), _float(ViewPort.Height), 0.f, 1.f);
    pVertex[2].vTexUV = _vec2(1.f, 1.f);

    pVertex[3].vPosition = _vec4(0.f, _float(ViewPort.Height), 0.f, 1.f);
    pVertex[3].vTexUV = _vec2(0.f, 1.f);

    m_pVB->Unlock();


    INDEX16* pIndex = NULL;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    pIndex[0]._0 = 0;
    pIndex[0]._1 = 1;
    pIndex[0]._2 = 2;

    pIndex[1]._0 = 0;
    pIndex[1]._1 = 2;
    pIndex[1]._2 = 3;

    m_pIB->Unlock();

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

    m_pManagement->Begin_MRT(L"MRT_Deferred");

    if (FAILED(Render_Priority())) 
    {
        return E_FAIL;
    }

    if (FAILED(Render_Priority_Alpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_NoAlpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_Alpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_HalfAlpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_Wireframe()))
    {
        return E_FAIL;
    }
    
    if (FAILED(Render_Effect()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_Terminal_NoAlpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_NO_ZEnable_NoAlpha()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_UI()))
    {
        return E_FAIL;
    } 

    if (FAILED(Render_UI_AlphaBlend()))
    {
        return E_FAIL;
    }

    if (FAILED(Render_Scene()))
    {
        return E_FAIL;
    }

    m_pManagement->End_MRT(L"MRT_Deferred");
    //랜더 리스트 끝

    Render_Original();
   
    if (m_bVisuableDebug == true)
    {
        m_pManagement->Render_DebugBuffer(L"MRT_Deferred");        
    }



    if (FAILED(CGraphicDevice::Get_Instance()->Render_End(hWND)))
    {
        PRINT_LOG(L"Render Error", L"Failed to Render_End (maybe occur m_pDevice Failed)");
        return E_FAIL;
    }

    return S_OK;
}

void CRenderer::Set_Visualble_DebugBuffer(_bool bVisual)
{
    m_bVisuableDebug = bVisual;
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


    ////알파블랜딩 먹임

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
	{
		return E_FAIL;
	}
	//알파블랜딩 옵션 적용
	if (FAILED(m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD)))
	{
		return E_FAIL;
	}
	//Source픽셀(=그려야 할 픽셀) 의 혼합 비율 설정
	if (FAILED(m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
	{
		return E_FAIL;
	}
	//Dest픽셀의 혼합 비율 설정
    //D3DRS_DESTBLEND: Dest픽셀(이미 그려져있는 픽셀)
    //D3DBLEND_INVSRCALPHA: 혼합비율이며 1 - D3DBLEND_SRCALPHA. 0~1범위.
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
    if (m_GameObjects[(_uint)RENDERID::RENDER_HALFALPHA].empty() == true)
    {
        return S_OK;
    }

    ////알파블랜딩 먹임
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
    {
        return E_FAIL;
    }
    //알파블랜딩 옵션 적용
    if (FAILED(m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD)))
    {
        return E_FAIL;
    }
    //Source픽셀(=그려야 할 픽셀) 의 혼합 비율 설정
    if (FAILED(m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA)))
    {
        return E_FAIL;
    }
    if (FAILED(m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR)))
    {
        return E_FAIL;
    }


    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_ALPHA])
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
    //얘는 알파테스팅

    if (m_GameObjects[(_uint)RENDERID::RENDER_ALPHA].empty())
    {
        return S_OK;
    }
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 128)))         //알파값 조절시 얘 바꾸면 됨
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER)))
    {
        return E_FAIL;
    }

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_ALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
            return E_FAIL;

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_ALPHA].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Render_Wireframe()
{
    //연산 많이 처먹으니까 오브젝트 하나도 없으면 생략

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

//빌보드 설정까지 같이 넣기?
HRESULT CRenderer::Render_Effect()
{    

    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
    {
        return E_FAIL;
    }

        ////알파블랜딩 먹임
    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
    {
        return E_FAIL;
    }

    m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCCOLOR);

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_EFFECT])
    {
        if (FAILED(pGameObject->Render_GameObject()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);   
    }

    m_GameObjects[(_uint)RENDERID::RENDER_EFFECT].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
    {
        return E_FAIL;
    }


    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Render_NO_ZEnable_NoAlpha()
{
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE)))
    {
        return E_FAIL;
    }
    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_NOZ_NOALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_NOZ_NOALPHA].clear();

    m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE)))
    {
        return E_FAIL;
    }

    
    return S_OK;
}

HRESULT CRenderer::Render_Terminal_NoAlpha()
{
 //http://telnet.or.kr/directx/graphics/reference/d3d/interfaces/idirect3ddevice9/clear.htm
 //   쉐이더 써야겠네;

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_TERMINAL_NOALPHA])
        {
            if (FAILED(pGameObject->Render_GameObject()))
                return E_FAIL;

            Safe_Release(pGameObject);
        }

    m_GameObjects[(_uint)RENDERID::RENDER_TERMINAL_NOALPHA].clear();

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
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHAREF, 128)))         //알파값 조절시 얘 바꾸면 됨
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
HRESULT CRenderer::Render_UI_AlphaBlend()
{
    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE)))
    {
        return E_FAIL;
    }

    ////알파블랜딩 먹임
    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
    {
        return E_FAIL;
    }
    m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    for (auto& pGameObject : m_GameObjects[(_uint)RENDERID::RENDER_UI_ALPHA])
    {
        if (FAILED(pGameObject->Render_GameObject()))
        {
            return E_FAIL;
        }

        Safe_Release(pGameObject);
    }

    m_GameObjects[(_uint)RENDERID::RENDER_UI_ALPHA].clear();

    if (FAILED(m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
    {
        return E_FAIL;
    }

    if (FAILED(m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE)))
    {
        return E_FAIL;
    }

    return S_OK;
}
//씬에서 남은 자잘구레한것들 출력
HRESULT CRenderer::Render_Scene()
{
    m_pManagement->Render_Scene();

    return S_OK;
}

HRESULT CRenderer::Render_Original()
{
    CShader* pShader = dynamic_cast<Engine::CShader*>(m_pManagement->Clone_Prototype(L"Shader_Original"));
    NULL_CHECK_RETURN(pShader,E_FAIL);

    LPD3DXEFFECT	pEffect = pShader->Get_EffectHandle();
    pEffect->AddRef();

    pEffect->Begin(NULL, 0);
    pEffect->BeginPass(0);

    m_pManagement->SetUp_OnShader(pEffect, L"Target_Original", "g_OriginalTexture");
    
    m_pDevice->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
    m_pDevice->SetFVF(FVF_SCREEN);
    m_pDevice->SetIndices(m_pIB);
    m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

    pEffect->EndPass();
    pEffect->End();

    Safe_Release(pShader);
    Safe_Release(pEffect);


    return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
    return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
    return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
    return S_OK;
}

HRESULT CRenderer::Clear_RenderList()
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


    Safe_Release(m_pIB);
    Safe_Release(m_pVB);

    Safe_Release(m_pManagement);
    Safe_Release(m_pDevice);

}
