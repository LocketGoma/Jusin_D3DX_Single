#include "VTXCubeColor.h"

USING(Engine)

CVTXCubeColor::CVTXCubeColor(_Device pDevice)
	: CVIBuffer(pDevice)
{
}

CVTXCubeColor::CVTXCubeColor(const CVTXCubeColor& other)
	: CVIBuffer(other)
{
	m_bIsPrototype = false;
}
//È÷È÷ ±ÍÂú¾Æ º¹ºÙÇÏ±â
HRESULT CVTXCubeColor::Ready_Buffer(void)
{
	m_dwVTXCount = 8;
	m_dwTriCount = 12;
	m_dwVTXSize = sizeof(VTX_CUBE_COLOR_N);
	m_dwFVF = FVF_CUBECOLORN;

	m_dwIndexSize = sizeof(INDEX16);
	m_IndexFormat = D3DFMT_INDEX16;

	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	VTX_CUBE_COLOR_N* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	// ¾Õ¸é
	pVertex[0].vPosition = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[1].vPosition = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[2].vPosition = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[3].vPosition = _vec3(-0.5f, -0.5f, -0.5f);
	
	pVertex[0].vNormal = _vec3(-0.5f, 0.5f, -0.5f);
	pVertex[1].vNormal = _vec3(0.5f, 0.5f, -0.5f);
	pVertex[2].vNormal = _vec3(0.5f, -0.5f, -0.5f);
	pVertex[3].vNormal = _vec3(-0.5f, -0.5f, -0.5f);

	// µÞ¸é
	pVertex[4].vPosition = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[5].vPosition = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[6].vPosition = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[7].vPosition = _vec3(-0.5f, -0.5f, 0.5f);

	pVertex[4].vNormal = _vec3(-0.5f, 0.5f, 0.5f);
	pVertex[5].vNormal = _vec3(0.5f, 0.5f, 0.5f);
	pVertex[6].vNormal = _vec3(0.5f, -0.5f, 0.5f);
	pVertex[7].vNormal = _vec3(-0.5f, -0.5f, 0.5f);

	for (int i = 0; i < 8; i++) {
		pVertex[i].wColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	}

	//m_pVertexArr = new VTX_CUBE_COLOR_N[m_dwVTXCount];
	//memcpy(m_pVertexArr, pVertex, m_dwVTXCount * m_dwVTXSize);

	m_pVB->Unlock();


	INDEX16* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// X+
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// X-
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;

	// y+
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// y-
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// z+
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// z-
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

HRESULT CVTXCubeColor::Render_Buffer(void)
{
	return CVIBuffer::Render_Buffer();
}

HRESULT CVTXCubeColor::Change_Color(D3DXCOLOR _Color)
{
	if (m_pVB == nullptr)
	{
		return E_FAIL;
	}
	VTX_CUBE_COLOR_N* pVertex = nullptr;
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for (int i = 0; i < 8; i++)
	{
		//pVertex[i].vPosition = ((VTX_CUBE_COLOR_N*)m_pVertexArr[i]).vPosition;
		pVertex[i].wColor = _Color;
	}

	m_pVB->Unlock();


	return S_OK;
}

CVTXCubeColor* CVTXCubeColor::Create(_Device pDevice)
{
	CVTXCubeColor* pInstance = new CVTXCubeColor(pDevice);

	if (FAILED(pInstance->Ready_Buffer()))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVTXCubeColor::Clone(void* pArg)
{
	return new CVTXCubeColor(*this);
}

void CVTXCubeColor::Free()
{
	CVIBuffer::Free();
}
