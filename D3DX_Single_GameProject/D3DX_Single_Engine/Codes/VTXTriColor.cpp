#include "VTXTriColor.h"

USING(Engine)

CVTXTriColor::CVTXTriColor(_Device pDevice)
    : CVIBuffer(pDevice)
{
	m_bIsPrototype = true;
}

CVTXTriColor::CVTXTriColor(const CVTXTriColor& other)
    : CVIBuffer(other)
{
	m_bIsPrototype = false;
}

HRESULT CVTXTriColor::Ready_Buffer(void)
{
	m_dwVTXCount = 3;
	m_dwVTXSize = sizeof(VTXCOL);
	m_dwTriCount = 1;
	m_dwFVF = FVF_COL;
	m_dwIndexSize = sizeof(INDEX32);
	m_IndexFormat = D3DFMT_INDEX32;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXCOL* pVtxCol = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCol, 0);
	// 모든 정점의 접근 권한을 잠궈버림, pVtxCol에게 모든 정점 중 첫 번째 주소를 추출하여 삽입

	pVtxCol[0].vPosition = _vec3(0.f, 0.5f, 0.f);
	pVtxCol[0].dwColor = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);

	pVtxCol[1].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVtxCol[1].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	pVtxCol[2].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVtxCol[2].dwColor = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);

	m_pVB->Unlock();

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVTXTriColor::Render_Buffer(void)
{
	return CVIBuffer::Render_Buffer();
}

CVTXTriColor* CVTXTriColor::Create(_Device pDevice)
{
	CVTXTriColor* pInstance = new CVTXTriColor(pDevice);

	if (FAILED(pInstance->Ready_Buffer()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CVTXTriColor::Clone(void* pArg)
{
	return new CVTXTriColor(*this);		//new?????
}

void CVTXTriColor::Free()
{
	CVIBuffer::Free();
}
