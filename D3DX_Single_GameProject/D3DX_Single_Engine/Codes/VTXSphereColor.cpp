#include "VTXSphereColor.h"

USING(Engine)

CVTXSphereColor::CVTXSphereColor(_Device pDevice, _float fRadius, _uint iSlice, _uint iStack, _vec4 vColor)
	: CVIBuffer(pDevice)
	, m_fRadius(fRadius)
	, m_iSlice(iSlice)
	, m_iStack(iStack)
	, m_vColor(vColor)
	, m_pMesh(nullptr)
{
	m_bIsPrototype = true;
}

CVTXSphereColor::CVTXSphereColor(const CVTXSphereColor& other)
	: CVIBuffer(other)
	, m_fRadius(other.m_fRadius)
	, m_iSlice(other.m_iSlice)
	, m_iStack(other.m_iStack)
	, m_vColor(other.m_vColor)
	, m_pMesh(other.m_pMesh)
{
	m_bIsPrototype = false;
}

HRESULT CVTXSphereColor::Ready_Buffer(void)
{
	D3DXCreateSphere(m_pDevice, m_fRadius, m_iSlice, m_iStack, &m_pMesh, NULL);

	LPDIRECT3DVERTEXBUFFER9* pViBuffer = nullptr;

	m_pMesh->GetVertexBuffer(pViBuffer);
	
	
	m_dwVTXCount = m_pMesh->GetNumVertices();
	m_dwTriCount = m_pMesh->GetNumFaces();

	Safe_Release(m_pMesh);

	m_dwVTXSize = sizeof(VTXCOL);
	m_dwFVF = FVF_COL;

	m_dwIndexSize = sizeof(INDEX32);
	m_IndexFormat = D3DFMT_INDEX32;

	VTXCOL* pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);


	pVertex[0].dwColor = D3DXCOLOR(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);
	

	for (_uint i = 1; i < m_iStack; i++)
	{
		_vec3 vPos(0.0f, (m_fRadius * cosf(D3DX_PI / m_iStack) * i), m_fRadius * sinf((D3DX_PI / m_iStack) * i));

		for (_uint j = 0; j < m_iSlice; j++) {
			_uint index = i * m_iSlice + j;

			pVertex[index].vPosition.x = (vPos.z * cosf(D3DXToRadian(360.f / m_iSlice) * j));
			pVertex[index].vPosition.y = vPos.y;
			pVertex[index].vPosition.x = (vPos.z * sinf(D3DXToRadian(360.f / m_iSlice) * j));

			//https://3dmpengines.tistory.com/747


			pVertex[index].dwColor = D3DXCOLOR(m_vColor.x, m_vColor.y, m_vColor.z, m_vColor.w);
		}

	}
	

	pVertex[0].vPosition.x = 0.0f;
	pVertex[0].vPosition.y = m_fRadius;
	pVertex[0].vPosition.z = 0.0f;

	m_pVB->Unlock();


	return E_NOTIMPL;
}

HRESULT CVTXSphereColor::Render_Buffer(void)
{
	return E_NOTIMPL;
}

HRESULT CVTXSphereColor::Change_Color(D3DXCOLOR _Color)
{
	return E_NOTIMPL;
}

CVTXSphereColor* CVTXSphereColor::Create(_Device pDevice, _float m_fRadius, _uint m_iSlice, _uint m_iStack, _vec4 vColor)
{
	return nullptr;
}

CComponent* CVTXSphereColor::Clone(void* pArg)
{
	return nullptr;
}

void CVTXSphereColor::Free()
{
	Safe_Release(m_pMesh);
}
