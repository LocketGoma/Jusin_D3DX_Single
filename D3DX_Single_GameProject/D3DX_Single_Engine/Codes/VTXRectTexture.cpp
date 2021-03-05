#include "VTXRectTexture.h"

USING(Engine)

CVTXRectTexture::CVTXRectTexture(_Device pDevice)
    : CVIBuffer(pDevice)
{
    m_bIsPrototype = true;
}

CVTXRectTexture::CVTXRectTexture(const CVTXRectTexture& other)
    : CVIBuffer(other)
{
    m_bIsPrototype = false;
}

HRESULT CVTXRectTexture::Ready_Buffer(void)
{
	m_dwVTXCount = 4;
	m_dwVTXSize = sizeof(VTXTEX);
	m_dwTriCount = 2;
	m_dwFVF = FVF_TEX;
	m_dwIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTXTEX* pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	// 모든 정점의 접근 권한을 잠궈버림, pVtxCol에게 모든 정점 중 첫 번째 주소를 추출하여 삽입

	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();


	return S_OK;
}

HRESULT CVTXRectTexture::Render_Buffer(void)
{
    return 	CVIBuffer::Render_Buffer();
}

CVTXRectTexture* CVTXRectTexture::Create(_Device pDevice)
{
	CVTXRectTexture* pInstance = new CVTXRectTexture(pDevice);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVTXRectTexture::Clone(void* pArg)
{
	return new CVTXRectTexture(*this);
}

void CVTXRectTexture::Free()
{
	CVIBuffer::Free();
}

HRESULT CVTXRectTexture::Change_Color(D3DXCOLOR _Color)
{
	return E_FAIL;
}
