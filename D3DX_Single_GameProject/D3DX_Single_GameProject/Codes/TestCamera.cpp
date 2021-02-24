#include "framework.h"
#include "TestCamera.h"

#include "Transform.h"
#include "CameraComponent.h"

CTestCamera::CTestCamera(_Device pDevice)
    : Engine::CGameObject(pDevice)
{
}


HRESULT CTestCamera::Ready_GameObject(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);


    return S_OK;
}

_int CTestCamera::Update_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::Update_GameObject(fTimeDelta);

    Key_Input(fTimeDelta);

   // m_pCameraCom->Set_ViewVector(m_pCameraCom->Get_CameraDesc().vEye + m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS), m_pCameraCom->Get_CameraDesc().vAt + m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK), m_pCameraCom->Get_CameraDesc().vUp + m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP));
    m_pCameraCom->Set_ViewVector(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS)+_vec3(0.f,0.f,-5.f), m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK), m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP));

    return 0;
}

_int CTestCamera::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_pTransformCom->Update_Component(fTimeDelta);
    m_pTransformCom->LateUpdate_Component(0.f);

    return m_pCameraCom->LateUpdate_Component(fTimeDelta);
}

//뭐 스카이박스할때나...
HRESULT CTestCamera::Render_GameObject(void)
{

    return S_OK;
}

HRESULT CTestCamera::Add_Component(void)
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return 0;
    }

    Engine::CComponent* pComponent = nullptr;

    pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(pManagement->Clone_Prototype(L"Transform_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Transform", pComponent);

    pComponent = m_pCameraCom = dynamic_cast<Engine::CCameraComponent*>(pManagement->Clone_Prototype(L"Camera_Comp"));
    NULL_CHECK_RETURN(pComponent, E_FAIL);
    m_mapComponent[0].emplace(L"Com_Camera", pComponent);


    return S_OK;
}

void CTestCamera::Key_Input(const _float& fTimeDelta)
{
    auto pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return;
    }


    if (pManagement->Key_Pressing('W'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)), 10.f, fTimeDelta);
    }

    if (pManagement->Key_Pressing('S'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)), 10.f, -fTimeDelta);
    }
    if (pManagement->Key_Pressing('A'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, -fTimeDelta);
    }
    if (pManagement->Key_Pressing('D'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, fTimeDelta);
    }
    
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    ScreenToClient(g_hWnd, &ptMouse);

    //뷰포트 -> 투영
    _float3 vMouse = _float3(0.f, 0.f, 0.f);
    vMouse.x = ptMouse.x / (WINCX * 0.5f) - 1.f;
    vMouse.y = 1.f - ptMouse.y / (WINCY * 0.5f);


    //투영 -> 뷰
    _float4x4 matProj = m_pCameraCom->Get_CameraDesc().matProj;
    D3DXMatrixInverse(&matProj, 0, &matProj);
    D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

    //단순 이동
    m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), vMouse.x-0.5f, fTimeDelta*3.f);
    m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)), 0.5f - vMouse.y, fTimeDelta*3.f);

    //ptMouse.x = WINCX / 2;
    //ptMouse.y = WINCY / 2;
    //ClientToScreen(g_hWnd, &ptMouse);
    //SetCursorPos(ptMouse.x, ptMouse.y);

}

CTestCamera* CTestCamera::Create(_Device pDevice)
{
    CTestCamera* pInstance = new CTestCamera(pDevice);

    if (FAILED(pInstance->Ready_GameObject()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTestCamera::Free(void)
{
    //Safe_Release(m_pTransformCom);
    //Safe_Release(m_pCameraCom);

    Engine::CGameObject::Free();
}
