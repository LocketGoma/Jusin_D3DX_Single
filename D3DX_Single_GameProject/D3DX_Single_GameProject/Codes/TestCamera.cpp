#include "framework.h"
#include "TestCamera.h"

#include "Transform.h"
#include "CameraComponent.h"

CTestCamera::CTestCamera(_Device pDevice)
    : Engine::CGameObject(pDevice)
    , m_fRotate(0.f)
    , m_fAxisXSpeed(100.f)
    , m_fAxisYSpeed(100.f)
    , m_bMouseLock(true)
{
}

CTestCamera::CTestCamera(const CTestCamera& other)
    : Engine::CGameObject(other)
    , m_fRotate(other.m_fRotate)
    , m_fAxisXSpeed(other.m_fAxisXSpeed)
    , m_fAxisYSpeed(other.m_fAxisYSpeed)
    , m_bMouseLock(true)
{
}


HRESULT CTestCamera::Ready_GameObject(void)
{
    return S_OK;
}

HRESULT CTestCamera::Ready_GameObject_Clone(void* pArg)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    POINT ptMouse;
    ptMouse.x = WINCX / 2;
    ptMouse.y = WINCY / 2;
    ClientToScreen(g_hWnd, &ptMouse);
    SetCursorPos(ptMouse.x, ptMouse.y);


    m_fAxisXSpeed = 150.f;
    m_fAxisYSpeed = 100.f;

    return S_OK;
}

_int CTestCamera::Update_GameObject(const _float& fDeltaTime)
{
    Engine::CGameObject::Update_GameObject(fDeltaTime);

    Key_Input(fDeltaTime);

    if (m_bMouseLock == true)
    {
        Mouse_Movement();
    }

    m_pTransformCom->Update_Component(fDeltaTime);

    return NO_EVENT;
}

_int CTestCamera::LateUpdate_GameObject(const _float& fDeltaTime)
{
    m_pTransformCom->LateUpdate_Component(0.f);

    m_pCameraCom->Set_ViewVector(m_pTransformCom->
        Get_Info(Engine::TRANSFORM_INFO::INFO_POS) - m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)*5.f,
        m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS),
        m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP));

    return m_pCameraCom->LateUpdate_Component(fDeltaTime);
}

//뭐 스카이박스할때나...
HRESULT CTestCamera::Render_GameObject(void)
{

    return S_OK;
}

void CTestCamera::Set_Position(_vec3 vPos)
{
    m_pTransformCom->Set_Pos(vPos);
}

void CTestCamera::Set_Size(_vec3 vSize) //안씀
{
    ;
}

_vec3 CTestCamera::Get_Position()
{
    return m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_POS);
}

_vec3 CTestCamera::Get_Size()       //안씀
{
    return _vec3();
}

HRESULT CTestCamera::Add_Component(void)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
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

void CTestCamera::Key_Input(const _float& fDeltaTime)
{
    Engine::CManagement* pManagement = Engine::CManagement::Get_Instance();
    if (nullptr == pManagement)
    {
        return;
    }


    if (pManagement->Key_Pressing('W'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)), 10.f, fDeltaTime);
    }
    if (pManagement->Key_Pressing('S'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)), 10.f, -fDeltaTime);
    }
    if (pManagement->Key_Pressing('A'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, -fDeltaTime);
    }
    if (pManagement->Key_Pressing('D'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), 10.f, fDeltaTime);
    }
    if (pManagement->Key_Pressing('Q'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, -fDeltaTime);
    }
    if (pManagement->Key_Pressing('E'))
    {
        m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_LOOK)), 10.f, fDeltaTime);
    }
    if (pManagement->Key_Pressing(VK_OEM_3))        // = '~'
    {
        m_bMouseLock = m_bMouseLock == false;
    } 

}
void CTestCamera::Mouse_Movement()
{
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
    //m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_RIGHT)), -vMouse.x, fDeltaTime*25.f);
    //m_pTransformCom->Move_Pos(&(m_pTransformCom->Get_Info(Engine::TRANSFORM_INFO::INFO_UP)),  -vMouse.y, fDeltaTime*30.f);

    //m_fRotate += vMouse.x * fDeltaTime;

    m_pTransformCom->Rotation(Engine::ROTATION::ROT_Y, vMouse.x * (m_fAxisXSpeed/100.f));
    m_pTransformCom->Rotation(Engine::ROTATION::ROT_X, -vMouse.y * (m_fAxisYSpeed / 100.f));


    ////TCHAR msg[256] = L"";
    ////TCHAR Omsg[] = L"Test Stage :: Mouse Pos -> %2.2f, %2.2f";

    ////swprintf_s(msg, Omsg, ptMouse.x, ptMouse.y);
    ////SetWindowText(g_hWnd, msg);


    ptMouse.x = WINCX >> 1;
    ptMouse.y = WINCY >> 1;
    ClientToScreen(g_hWnd, &ptMouse);
    SetCursorPos(ptMouse.x, ptMouse.y);
    

}
//마우스 피킹 구현용
void CTestCamera::Picking_Mouse()
{
}

void CTestCamera::Set_MouseSpeedX(_float fAxisX)
{
    m_fAxisXSpeed = fAxisX;
}

void CTestCamera::Set_MouseSpeedY(_float fAxisY)
{
    m_fAxisYSpeed = fAxisY;
}

_float CTestCamera::Get_MouseSpeedX()
{
    return m_fAxisXSpeed;
}

_float CTestCamera::Get_MouseSpeedY()
{
    return m_fAxisYSpeed;
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

Engine::CGameObject* CTestCamera::Clone(void* pArg)
{
    CTestCamera* pClone = new CTestCamera(*this);

    if (pClone == nullptr)
    {
        PRINT_LOG(L"Error", L"Failed To Clone CTestCamera");
    }
    if (FAILED(pClone->Ready_GameObject_Clone(pArg)))
    {
        Safe_Release(pClone);
    }


    return pClone;
}

void CTestCamera::Free(void)
{
    //Safe_Release(m_pTransformCom);
    //Safe_Release(m_pCameraCom);

    Engine::CGameObject::Free();
}
