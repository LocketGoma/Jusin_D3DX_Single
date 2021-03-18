#include "VTXTerrain.h"

using namespace Engine;
//= USING(Engine)

CVTXTerrain::CVTXTerrain(_Device pDevice)
    : CVIBuffer(pDevice)
    , m_pPos(nullptr)
{
    m_bIsPrototype = true;
}

CVTXTerrain::CVTXTerrain(const CVTXTerrain& other)
    : CVIBuffer(other)
    , m_pPos(other.m_pPos)
    , m_fH(other.m_fH)
    , m_iH(other.m_iH)
    , m_dwCountX(other.m_dwCountX)
    , m_dwCountZ(other.m_dwCountZ)
{
    m_bIsPrototype = false;
}

const _vec3* CVTXTerrain::Get_VTXPos(void) const
{
    return m_pPos;
}

_ulong CVTXTerrain::Get_VTXCountX() const
{
    return m_dwCountX;
}

_ulong CVTXTerrain::Get_VTXCountZ() const
{
    return m_dwCountZ;
}
//나중에!
void CVTXTerrain::Copy_Index(INDEX32* pIndex, const _ulong& dwTriCount)
{
}

HRESULT CVTXTerrain::Ready_Buffer(const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
    m_dwTriCount = (dwCountX - 1) * (dwCountZ - 1) * 2; //삼각형개수
    m_dwVTXCount = dwCountX * dwCountZ;
    m_dwVTXSize = sizeof(VTXTEX);
    m_dwIndexSize = sizeof(INDEX32);
    m_IndexFormat = D3DFMT_INDEX32;
    m_dwFVF = FVF_TEX;

    m_pPos = new _vec3[m_dwVTXCount];
    m_dwCountX = dwCountX;
    m_dwCountZ = dwCountZ;


    FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

    //높이맵 지정

    _ulong		dwByte = 0;
    _ulong* pPixel;

    m_hFile = CreateFile(L"../../Resource/TestResource/Texture/Terrain/Height.bmp",
        GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (m_hFile != INVALID_HANDLE_VALUE)
    {
        ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
        ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

        pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

        ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biWidth * m_iH.biHeight, &dwByte, NULL);
    }
    else
    {
        pPixel = new _ulong[(dwCountX) * (dwCountZ)]{};
    }
    CloseHandle(m_hFile);

    VTXTEX* pVertex = NULL;

    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    _ulong	dwIndex = 0;

    for (_ulong i = 0; i < dwCountZ; ++i)
    {
        for (_ulong j = 0; j < dwCountX; ++j)
        {
            dwIndex = i * dwCountX + j;

            pVertex[dwIndex].vPosition = _vec3(_float(j) * dwVTXInterval,
                (pPixel[dwIndex] & 0x000000ff) / 20.f,
                _float(i) * dwVTXInterval);

            m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

            pVertex[dwIndex].vTexUV = _vec2(_float(j) / (dwCountX - 1) * 20.f,
                _float(i) / (dwCountZ - 1) * 20.f);

            pVertex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);
        }
    }
    m_pVB->Unlock();

    Safe_Delete_Array(pPixel);

    //높이맵 지정 끝

    //인덱스 지정
    INDEX32* pIndex = NULL;

    m_pIB->Lock(0, 0, (void**)&pIndex, 0);

    _ulong	dwTriIndex = 0;

    for (_ulong i = 0; i < dwCountZ - 1; ++i)
    {
        for (_ulong j = 0; j < dwCountX - 1; ++j)
        {
            dwIndex = i * dwCountX + j;

            // 오른쪽 위
            pIndex[dwTriIndex]._0 = dwIndex + dwCountX;
            pIndex[dwTriIndex]._1 = dwIndex + dwCountX + 1;
            pIndex[dwTriIndex]._2 = dwIndex + 1;
            dwTriIndex++;

            // 왼쪽 아래
            pIndex[dwTriIndex]._0 = dwIndex + dwCountX;
            pIndex[dwTriIndex]._1 = dwIndex + 1;
            pIndex[dwTriIndex]._2 = dwIndex;
            dwTriIndex++;
        }
    }

    m_pIB->Unlock();

    //인덱스 지정 끝

    //노멀 벡터 지정 (느려지면 함수 위치 바꿀것)
    m_pVB->Lock(0, 0, (void**)&pVertex, 0);

    for (_ulong i = 0; i < dwTriIndex; i++)
    {
        _vec3 p1, p2, pNormal;
        p1 = pVertex[pIndex[i]._1].vPosition - pVertex[pIndex[i]._0].vPosition;
        p2 = pVertex[pIndex[i]._2].vPosition - pVertex[pIndex[i]._0].vPosition;

        D3DXVec3Cross(&pNormal, &p1, &p2);
        D3DXVec3Normalize(&pNormal, &pNormal);

        pVertex[pIndex[i]._0].vNormal += pNormal;
        pVertex[pIndex[i]._1].vNormal += pNormal;
        pVertex[pIndex[i]._2].vNormal += pNormal;
        
        //노멀라이즈를 몇번하는거야;
        D3DXVec3Normalize(&pVertex[pIndex[i]._0].vNormal, &pVertex[pIndex[i]._0].vNormal);
        D3DXVec3Normalize(&pVertex[pIndex[i]._1].vNormal, &pVertex[pIndex[i]._1].vNormal);
        D3DXVec3Normalize(&pVertex[pIndex[i]._2].vNormal, &pVertex[pIndex[i]._2].vNormal);
    }

    m_pVB->Unlock();

    //노멀 벡터 지정 끝


    return S_OK;
}

HRESULT CVTXTerrain::Render_Buffer(void)
{
    return CVIBuffer::Render_Buffer();
}

CVTXTerrain* CVTXTerrain::Create(_Device pDevice, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
    CVTXTerrain* pInstance = new CVTXTerrain(pDevice);

    if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ,dwVtxItv)))
        Safe_Release(pInstance);

    return pInstance;
}

CComponent* CVTXTerrain::Clone(void* pArg)
{
    return new CVTXTerrain(*this);
}

void CVTXTerrain::Free()
{
    if (true == m_bIsPrototype)
        Safe_Delete_Array(m_pPos);

    CVIBuffer::Free();
}

HRESULT CVTXTerrain::Change_Color(D3DXCOLOR _Color)
{
    return E_FAIL;
}
