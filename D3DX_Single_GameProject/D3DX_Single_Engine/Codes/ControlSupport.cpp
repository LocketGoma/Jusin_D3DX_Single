#include "ControlSupport.h"
#include "VIBuffer.h"
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

_vec3 CControlSupportUnit::Picking_Object(HWND hWnd, const CVIBuffer* pBuffer, const CTransform* pTransform)
{
    return _vec3();
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
