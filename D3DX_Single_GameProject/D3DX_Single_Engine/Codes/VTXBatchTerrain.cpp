#include "VTXTerrain.h"
#include "VTXBatchTerrain.h"

using namespace Engine;

CVTXBatchTerrain::CVTXBatchTerrain(_Device pDevice)
	: CVTXTerrain(pDevice)		
{
	m_bIsPrototype = true;
}

CVTXBatchTerrain::CVTXBatchTerrain(const CVTXBatchTerrain& other)
	: CVTXTerrain(other)

{
	m_bIsPrototype = false;
}

const _vec3* CVTXBatchTerrain::Get_VTXPos(void) const
{
	return m_pPos;
}

_ulong CVTXBatchTerrain::Get_VTXCountX() const
{
	return m_dwCountX;
}

_ulong CVTXBatchTerrain::Get_VTXCountZ() const
{
	return m_dwCountZ;
}

void CVTXBatchTerrain::Copy_Index(INDEX32* pIndex, const _ulong& dwTriCount)
{
	;
}

HRESULT CVTXBatchTerrain::Ready_Buffer(const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
	m_dwTriCount = (dwCountX - 1) * (dwCountZ - 1) * 2; //삼각형개수
	m_dwVTXCount = dwCountX * dwCountZ;
	m_dwVTXSize = sizeof(VTXCOL);
	m_dwIndexSize = sizeof(INDEX32);
	m_IndexFormat = D3DFMT_INDEX32;
	m_dwFVF = FVF_COL;

	m_pPos = new _vec3[m_dwVTXCount];
	m_dwCountX = dwCountX;
	m_dwCountZ = dwCountZ;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);


	VTXCOL* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	_ulong dwIndex = 0;
	_long dwCountHZ = dwCountZ >> 1;
	_long dwCountHX = dwCountX >> 1;
	_long dwInterval = dwVTXInterval;

	for (_long i = 0; i < dwCountZ; i++)
	{
		for (_long j = 0; j < dwCountX; j++)
		{
			dwIndex = i * dwCountX + j;
			pVertex[dwIndex].vPosition = _vec3(_float((j - (dwCountHX)) * dwInterval), 0.f, _float((i - (dwCountHZ)) * dwInterval));

			m_pPos[dwIndex] = pVertex[dwIndex].vPosition;

			pVertex[dwIndex].dwColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);


			if ((j - (dwCountHX)) == 0 && (i - (dwCountHZ)) == 0)
			{
				pVertex[dwIndex].dwColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else if ((j - (dwCountHX)) == 0)
			{
				pVertex[dwIndex].dwColor = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			}
			else if ((i - (dwCountHZ)) == 0)
			{
				pVertex[dwIndex].dwColor = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			}
		}
	}
	m_pVB->Unlock();

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


	return S_OK;
}

HRESULT CVTXBatchTerrain::Render_Buffer(void)
{
	return CVIBuffer::Render_Buffer();
}

HRESULT CVTXBatchTerrain::Change_Color(D3DXCOLOR _Color)
{
	return E_FAIL;
}

CVTXBatchTerrain* CVTXBatchTerrain::Create(_Device pDevice, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	CVTXBatchTerrain* pInstance = new CVTXBatchTerrain(pDevice);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CVTXBatchTerrain::Clone(void* pArg)
{
	return new CVTXBatchTerrain(*this);
}

void CVTXBatchTerrain::Free()
{
	if (true == m_bIsPrototype)
		Safe_Delete_Array(m_pPos);

	CVIBuffer::Free();
}
