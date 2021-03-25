#include "StaticMesh.h"

USING(Engine)

CStaticMesh::CStaticMesh(_Device pDevice)
	: CMesh(pDevice)
	, m_pAdjacency(nullptr)
	, m_pSubset(nullptr)
	, m_dwSubsetCnt(0)
	, m_pMtrl(nullptr)
	, m_ppTextures(nullptr)
	, m_pMesh(nullptr)
{
}

CStaticMesh::CStaticMesh(const CStaticMesh& other)
	: CMesh(other)
	, m_pMesh(other.m_pMesh)
	, m_pAdjacency(other.m_pAdjacency)
	, m_pSubset(other.m_pSubset)
	, m_dwSubsetCnt(other.m_dwSubsetCnt)
	, m_pMtrl(other.m_pMtrl)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[other.m_dwSubsetCnt];

	for (_ulong i = 0; i < other.m_dwSubsetCnt; ++i)
	{
		if (other.m_ppTextures[i] != nullptr)
		{
			m_ppTextures[i] = other.m_ppTextures[i];
			m_ppTextures[i]->AddRef();
		}
		else
		{
			m_ppTextures[i] = nullptr;
		}
	}

	m_pAdjacency->AddRef();
	m_pSubset->AddRef();
	m_pMesh->AddRef();
}

HRESULT CStaticMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	D3DLOCKED_RECT LockRect;
	D3DXCreateTexture(m_pDevice, 1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pSampleTexture);
	m_pSampleTexture->LockRect(0, &LockRect, NULL, 0);
	*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	m_pSampleTexture->UnlockRect(0);



	_tchar	szFullPath[256] = L"";
	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	if (FAILED(D3DXLoadMeshFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pDevice,
		&m_pAdjacency,
		&m_pSubset,
		NULL,
		&m_dwSubsetCnt,
		&m_pMesh)))
		return E_FAIL;

	// LPD3DXBUFFER가 담고 있는 버퍼 정보 중 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	////메쉬로부터 FVF 정보 가져옴.
	//_ulong	dwFVF = m_pMesh->GetFVF();

	//// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	//if (!(dwFVF & D3DFVF_NORMAL))
	//{
	//	PRINT_LOG(L"Warrning", L"Normal map not exist");

	//	m_pMesh->CloneMeshFVF(m_pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &m_pMesh);
	//	D3DXComputeNormals(m_pMesh, 0);
	//}


	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		if (m_pMtrl[i].pTextureFilename == nullptr)
		{
			m_ppTextures[i] = m_pSampleTexture;
			continue;
		}
		
		_tchar	szFileName[256] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP,
			0,
			m_pMtrl[i].pTextureFilename,
			strlen(m_pMtrl[i].pTextureFilename),
			szFileName,
			256);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &m_ppTextures[i])))
			return E_FAIL;
	}





	return S_OK;
}

void CStaticMesh::Render_Meshes()
{
	//전체 순회
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		if (m_ppTextures[i] != nullptr)
		{
			m_pDevice->SetTexture(0, m_ppTextures[i]);
		}
		else
		{

		}
		m_pMesh->DrawSubset(i);
	}
}

CStaticMesh* CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStaticMesh::Clone(void* pArg)
{
	return new CStaticMesh(*this);
}

void CStaticMesh::Free(void)
{
	//로드한거 다 죽여야됨
	Safe_Release(m_pSampleTexture);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubset);

	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);


	CMesh::Free();
}
