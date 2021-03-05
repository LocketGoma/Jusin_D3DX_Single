#include "CameraComponent.h"

USING(Engine)

CCameraComponent::CCameraComponent(_Device pDevice)
    : m_pDevice(pDevice)
    , m_bUseOrtho(false)
{
    ZeroMemory(&m_CameraDesc, sizeof(CCameraComponent::CAMERA_DESC));

    Safe_AddReference(m_pDevice);
}

CCameraComponent::CCameraComponent(const CCameraComponent& other)
    : CComponent(other)
    , m_pDevice(other.m_pDevice)
    , m_CameraDesc(other.m_CameraDesc)
    , m_bUseOrtho(other.m_bUseOrtho)
{

    Safe_AddReference(m_pDevice);

    m_bIsPrototype = false;
}

const CCameraComponent::CAMERA_DESC& CCameraComponent::Get_CameraDesc() const
{
    return m_CameraDesc;
}

void CCameraComponent::Set_CameraDesc(CAMERA_DESC& cDesc)
{
    m_CameraDesc = cDesc;
}

void CCameraComponent::Set_ViewVector(_vec3 vEye, _vec3 vAt, _vec3 vUp)
{
    m_CameraDesc.vEye = vEye;
    m_CameraDesc.vAt = vAt;
    m_CameraDesc.vUp = vUp;
}

void CCameraComponent::Set_Projection(_float fFovY, _float fAspect, _float fZnear, _float fZFar)
{
    m_CameraDesc.fFovY = fFovY;
    m_CameraDesc.fAspect = fAspect;
    m_CameraDesc.fZNear = fZnear;
    m_CameraDesc.fZFar = fZFar;
}

void CCameraComponent::Set_Ortho(_bool bUse)
{
    m_bUseOrtho = bUse;
}

HRESULT CCameraComponent::Ready_Camera()
{
    m_CameraDesc.vEye = _vec3(0.0f, 0.0f, -5.f);
    m_CameraDesc.vAt = _vec3(0.0f, 0.0f, 1.f);
    m_CameraDesc.vUp = _vec3(0.0f, 1.0f, 0.f);

    D3DXMatrixLookAtLH(&m_CameraDesc.matView, &m_CameraDesc.vEye, &m_CameraDesc.vAt, &m_CameraDesc.vUp);

    m_CameraDesc.fFovY = D3DXToRadian(60.f);
    m_CameraDesc.fAspect = _float(WINCX) / _float(WINCY);
    m_CameraDesc.fZNear = 1.f;
    m_CameraDesc.fZFar = 1000.f;

    D3DXMatrixPerspectiveFovLH(&m_CameraDesc.matProj, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fZNear, m_CameraDesc.fZFar);

    m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView);
    m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj);

    return S_OK;
}

_int CCameraComponent::Update_Component(const _float& fDeltaTime)
{
    return _int();
}

_int CCameraComponent::LateUpdate_Component(const _float& fDeltaTime)
{
    //뷰 행렬 세팅
    D3DXMatrixLookAtLH(&m_CameraDesc.matView, &m_CameraDesc.vEye, &m_CameraDesc.vAt, &m_CameraDesc.vUp);

    FAILED_CHECK_RETURN((m_pDevice->SetTransform(D3DTS_VIEW, &m_CameraDesc.matView)),E_FAIL);


    if (m_bUseOrtho == true)
    {
        //원근행렬 세팅
        D3DXMatrixOrthoLH(
            &m_CameraDesc.matOrtho, /* 직교투영행렬 반환 */
            WINCX, // 가로폭 (WINCX, 하드코딩한거 반드시 바꿀것)
            WINCY, // 세로폭 (WINCY)
            m_CameraDesc.fZNear, /* Near Z: 관찰자와 가장 가까운 면과의 Z거리 */
            m_CameraDesc.fZFar /* Far Z: 관찰자와 가장 먼 편과의 Z거리 */);        

        FAILED_CHECK_RETURN((m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matOrtho)), E_FAIL);
    }
    else
    {
        //투영행렬 세팅
        D3DXMatrixPerspectiveFovLH(&m_CameraDesc.matProj, m_CameraDesc.fFovY, m_CameraDesc.fAspect, m_CameraDesc.fZNear, m_CameraDesc.fZFar);

        FAILED_CHECK_RETURN((m_pDevice->SetTransform(D3DTS_PROJECTION, &m_CameraDesc.matProj)), E_FAIL);
    }
    

    return 0;
}

CCameraComponent* CCameraComponent::Create(_Device pDevice)
{
    CCameraComponent* pIstance = new CCameraComponent(pDevice);
    if (pIstance == nullptr)
    {
        return nullptr;
    }
    pIstance->Ready_Camera();


    return pIstance;
}

CComponent* CCameraComponent::Clone(void* pArg)
{
    CCameraComponent* pClone = new CCameraComponent(*this);
    if (FAILED(pClone->Ready_Camera()))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CCamera");
        Safe_Release(pClone);
    }

    return pClone;
}

void CCameraComponent::Free(void)
{
    Safe_Release(m_pDevice);
}
