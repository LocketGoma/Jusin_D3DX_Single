#include "VIBuffer.h"

USING(Engine)

CVIBuffer::CVIBuffer(_Device pDevice)
	: CGraphicResources(pDevice)
	, m_pVB(nullptr)
	, m_pIB(nullptr)
	,m_dwVTXSize(0)
	,m_dwVTXCount(0)
	,m_dwFVF(0)
	,m_dwIndexSize(0)
	,m_dwTriCount(0)
	,m_IndexFormat(_D3DFORMAT::D3DFMT_A8R8G8B8)
{
	m_bIsPrototype = true;
}

CVIBuffer::CVIBuffer(const CVIBuffer& other)
	: CGraphicResources(other)
	, m_pVB(other.m_pVB)
	, m_pIB(other.m_pIB)
	, m_dwVTXSize(other.m_dwVTXSize)
	, m_dwVTXCount(other.m_dwVTXCount)
	, m_dwFVF(other.m_dwFVF)
	, m_dwIndexSize(other.m_dwIndexSize)
	, m_dwTriCount(other.m_dwTriCount)
	, m_IndexFormat(other.m_IndexFormat)
{
	m_bIsPrototype = false;
	m_pVB->AddRef();
	m_pIB->AddRef();
}

_ulong CVIBuffer::Get_TriCount()
{
	return m_dwTriCount;
}

HRESULT CVIBuffer::Ready_Buffer()
{
	FAILED_CHECK_RETURN(m_pDevice->CreateVertexBuffer(m_dwVTXSize * m_dwVTXCount,
		0, // 정적 버퍼 사용 시 숫자 0(D3DUSAGE_DYNAMIC : 파티클을 생성할 때)
		m_dwFVF,
		D3DPOOL_MANAGED, // 일반적으로 정적 버퍼 사용 시 Managed, 동적버퍼 사용 시 Default 사용
		&m_pVB, NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pDevice->CreateIndexBuffer(m_dwIndexSize * m_dwTriCount,
		0,
		m_IndexFormat,
		D3DPOOL_MANAGED,
		&m_pIB, NULL), E_FAIL);

	return S_OK;
}

void CVIBuffer::Render_Buffer()
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVTXSize);

	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->SetIndices(m_pIB);

	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVTXCount, 0, m_dwTriCount);
}

void CVIBuffer::Free(void)
{
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CGraphicResources::Free();
}
