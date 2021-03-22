#include "ControlSupport.h"
#include "VIBuffer.h"
#include "VTXTerrain.h"
#include "Transform.h"

USING(Engine)

CControlSupportUnit::CControlSupportUnit(_Device pDevice)
    : m_pDevice(pDevice)
{
    m_bIsPrototype = true;

    

    Safe_AddReference(m_pDevice);
}

CControlSupportUnit::CControlSupportUnit(const CControlSupportUnit& other)
    : CComponent(other)
    , m_pDevice(other.m_pDevice)
{
    m_bIsPrototype = false;

    

    Safe_AddReference(m_pDevice);
}

HRESULT CControlSupportUnit::Ready_SupportUnit()
{
    return S_OK;
}

//�ٵ� ���͹��� �� ulong����?
_float CControlSupportUnit::Calculate_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVTX, const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
    //(Z�� ���� ��ġ / �� �ͷ��κ� ����) * X�� ���� + (X�� ���� ��ġ / �� �ͷ��κ� ����);
    _ulong dwIndex = _ulong(pPos->z / dwVTXInterval) * dwCountX + _ulong(pPos->x / dwVTXInterval);

    //�������� ����. ���ܿ��� �������� ���� -> �� ��ġ - ���� ��
    _float vRatioX = (pPos->x - pTerrainVTX[dwIndex+dwCountX].x)/dwVTXInterval;
    //�������� ����. ���ܿ��� �Ʒ��� ���� -> ���� �� - �� ��ġ
    _float vRatioZ = (pTerrainVTX[dwIndex+dwCountX].z - pPos->z)/dwVTXInterval;

    //0 : ���ϴ�
    //1 : ���ϴ�
    //2 : ����
    //3 : �»��
    _float fHeight[4] = { pTerrainVTX[dwIndex].y,pTerrainVTX[dwIndex + 1].y,pTerrainVTX[dwIndex + dwCountX + 1].y,pTerrainVTX[dwIndex + dwCountX].y, };
    
    _float fResult = fHeight[3];

    //����
    if (vRatioX > vRatioZ)
    {
        fResult = fHeight[3] + (fHeight[2] - fHeight[3]) * vRatioX + (fHeight[1] - fHeight[2]) * vRatioZ;
    }
    //���ϴ�
    if (vRatioX < vRatioZ)
    {
        fResult = fHeight[3] + (fHeight[1] - fHeight[0]) * vRatioX + (fHeight[0] - fHeight[3]) * vRatioZ;

    }

    return fResult;
}

//�̰� �߷°� ������� �ݵ�� �������ߵ�.
_vec3 CControlSupportUnit::Picking_Object(HWND hWnd, const CVIBuffer* pBuffer,const CTransform* pTransform)
{
    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(hWnd, &ptMouse);

    _vec3 vMousePos;

    //����Ʈ ȹ��
    D3DVIEWPORT9 pViewPort;
    ZeroMemory(&pViewPort, sizeof(_D3DVIEWPORT9));
    m_pDevice->GetViewport(&pViewPort);

    //������ ��ǥ -> ���� ��ǥ
    vMousePos.x = ptMouse.x / (pViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(pViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    //���� ��ǥ -> �� ��ǥ
    _mat matProj;
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, NULL, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    //�� ������ ���콺 ��ǥ�� �信 ����.

    //�� ��ǥ -> ���� ��ǥ
    _mat matView;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);


    //RayPos : ���� ��ŸƮ / RayDir : ���� ����
    _vec3 vRayPos, vRayDir;

    vRayPos = _vec3(0.f, 0.f, 0.f); //���� ���� : ȭ�� ����
    vRayDir = vMousePos - vRayPos; //���� ���� : ���콺 Ŭ����ġ

    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);






    return _vec3();
}

_vec3 CControlSupportUnit::Picking_Terrain(HWND hWnd, const CVTXTerrain* pBuffer, const CTransform* pTransform)
{
    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(hWnd, &ptMouse);

    _vec3 vMousePos;

    //����Ʈ ȹ��
    D3DVIEWPORT9 pViewPort;
    ZeroMemory(&pViewPort, sizeof(_D3DVIEWPORT9));
    m_pDevice->GetViewport(&pViewPort);

    //������ ��ǥ -> ���� ��ǥ
    vMousePos.x = ptMouse.x / (pViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(pViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    //���� ��ǥ -> �� ��ǥ
    _mat matProj;
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, NULL, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    //�� ������ ���콺 ��ǥ�� �信 ����.

    //�� ��ǥ -> ���� ��ǥ
    _mat matView;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);


    //RayPos : ���� ��ŸƮ / RayDir : ���� ����
    _vec3 vRayPos, vRayDir;
    
    vRayPos = _vec3(0.f, 0.f, 0.f); //���� ���� : ȭ�� ����
    vRayDir = vMousePos - vRayPos; //���� ���� : ���콺 Ŭ����ġ

    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    //���� -> ����
    //���� : ���ؽ� ������ ���������ϱ�.
    _mat matWorld;
    memcpy(&matWorld, pTransform->Get_TransformDescription().matWorld, sizeof(_mat));
    D3DXMatrixInverse(&matWorld, NULL, &matWorld);

    //Coord : ��ġ / Normal : ����
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

    //��ŷ�� Ÿ�� �ͷ��� ȹ��
    _ulong dwVTXCountX = pBuffer->Get_VTXCountX();
    _ulong dwVTXCountZ = pBuffer->Get_VTXCountZ();
    const _vec3* pTerrainVTX = pBuffer->Get_VTXPos();

    _float fU, fV, fDist;
    _ulong dwVTXIndex[3];

    _vec3 vReturnPos = _vec3(0.f, 0.f, 0.f);

    for (_ulong i = 0; i < dwVTXCountZ - 1; i++)
    {
        for (_ulong j = 0; j < dwVTXCountX - 1; j++)
        {
            _ulong dwIndex = i * dwVTXCountX + j;

            //������ ��
            dwVTXIndex[0] = dwIndex + dwVTXCountX;
            dwVTXIndex[1] = dwIndex + dwVTXCountX + 1;
            dwVTXIndex[2] = dwIndex + 1;

            if (D3DXIntersectTri(&pTerrainVTX[dwVTXIndex[1]], &pTerrainVTX[dwVTXIndex[2]], &pTerrainVTX[dwVTXIndex[0]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
            {
                vReturnPos = _vec3(pTerrainVTX[dwVTXIndex[1]] + (pTerrainVTX[dwVTXIndex[2]] - pTerrainVTX[dwVTXIndex[1]])*fU + (pTerrainVTX[dwVTXIndex[0]]- pTerrainVTX[dwVTXIndex[1]])*fV);

                break;
            }

            //���� �Ʒ�
            dwVTXIndex[0] = dwIndex + dwVTXCountX;
            dwVTXIndex[1] = dwIndex + 1;
            dwVTXIndex[2] = dwIndex;

            if (D3DXIntersectTri(&pTerrainVTX[dwVTXIndex[2]], &pTerrainVTX[dwVTXIndex[0]], &pTerrainVTX[dwVTXIndex[1]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
            {
                // = v0 + (u*(v1-v0))+(v*(v2-v0));

                vReturnPos = _vec3(pTerrainVTX[dwVTXIndex[2]] + (pTerrainVTX[dwVTXIndex[0]] - pTerrainVTX[dwVTXIndex[2]]) * fU + (pTerrainVTX[dwVTXIndex[1]] - pTerrainVTX[dwVTXIndex[2]]) * fV);

                break;
            }
        }
    }
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
    {
        vReturnPos.x = (float)((int)(vReturnPos.x + 0.5f));
        vReturnPos.y = (float)((int)(vReturnPos.y + 0.5f));
        vReturnPos.z = (float)((int)(vReturnPos.z + 0.5f));
    }

       
    //������ �ʱⰪ
    return vReturnPos;
}

CControlSupportUnit* CControlSupportUnit::Create(_Device pDevice)
{
    CControlSupportUnit* pInstance = new CControlSupportUnit(pDevice);

    if (FAILED(pInstance->Ready_SupportUnit()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CControlSupportUnit::Clone(void* pArg)
{
    return new CControlSupportUnit(*this);
}

void CControlSupportUnit::Free()
{
    Safe_Release(m_pDevice);
}
