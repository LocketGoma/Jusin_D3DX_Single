#include "CameraComponent.h"

USING(Engine)

CCameraComponent::CCameraComponent(_Device pDevice)
    : m_pDevice(pDevice)
{
    ZeroMemory(&m_CameraDesc, sizeof(CCameraComponent::CAMERA_DESC));
}

CCameraComponent::CCameraComponent(const CCameraComponent& other)
    : CComponent(other)
    , m_pDevice(other.m_pDevice)
    , m_CameraDesc(other.m_CameraDesc)
{

    Safe_AddReference(m_pDevice);

    m_bIsPrototype = false;
}

const CCameraComponent::CAMERA_DESC& CCameraComponent::Get_CameraDesc() const
{
    return m_CameraDesc;
}

HRESULT CCameraComponent::Ready_Camera()
{
    m_CameraDesc.vEye = _vec3(0.0f, 0.0f, -0.5f);
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

_int CCameraComponent::Update_Component(const _float& fTimeDelta)
{
    return _int();
}

_int CCameraComponent::LateUpdate_Component(const _float& fTimeDelta)
{
    return _int();
}

CComponent* CCameraComponent::Clone(void* pArg = nullptr)
{
    CCameraComponent* pClone = new CCameraComponent(*this);
    if (FAILED(pClone->Ready_Camera()))
    {
        PRINT_LOG(L"Error", L"Failed To Clone CTransform");
        Safe_Release(pClone);
    }

    return pClone;
}

void CCameraComponent::Free(void)
{
    Safe_Release(m_pDevice);
}
