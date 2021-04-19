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
	, m_pOriMesh(other.m_pOriMesh)
	, m_dwVtxCnt(other.m_dwVtxCnt)
	, m_dwStride(other.m_dwStride)
	, m_pVtxPos(other.m_pVtxPos)
	, m_pSampleTexture(other.m_pSampleTexture)
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
	m_pOriMesh->AddRef();
	m_pSampleTexture->AddRef();
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
		&m_pOriMesh)))
		return E_FAIL;

	// LPD3DXBUFFER가 담고 있는 버퍼 정보 중 가장 앞 주소를 리턴하는 함수
	m_pMtrl = (D3DXMATERIAL*)m_pSubset->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	////메쉬로부터 FVF 정보 가져옴.
	_ulong	dwFVF = m_pOriMesh->GetFVF();

	//// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		PRINT_LOG(L"Warrning", L"Normal map not exist");

		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL, m_pDevice, &m_pMesh);
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
		m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pDevice, &m_pMesh);
	}

	// 메쉬 사이즈에 맞는 바운딩박스 생성을 위한 작업
	void* pVertex = NULL;

	m_dwVtxCnt = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwVtxCnt];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	WORD		byOffSet = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffSet = Decl[i].Offset;
			break;
		}
	}

	// 정점이 지닌 fvf정보를 기준으로 정점의 크기를 계산하자
	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (i * m_dwStride + byOffSet)));
	}

	m_pMesh->UnlockVertexBuffer();



	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		if (m_pMtrl[i].pTextureFilename == nullptr)
		{
			m_ppTextures[i] = m_pSampleTexture;
			m_ppTextures[i]->AddRef();
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
		m_pDevice->SetTexture(0, m_ppTextures[i]);		
		m_pMesh->DrawSubset(i);
	}
}

void CStaticMesh::Render_Meshes(LPD3DXEFFECT pEffect)
{
	//전체 순회
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);
	}
}

const LPD3DXMESH* CStaticMesh::Get_VertrxInfo() const
{
	return &m_pMesh;
}

const _vec3* CStaticMesh::Get_VtxPos(void) const
{
	return m_pVtxPos;
}

_ulong CStaticMesh::Get_VtxCnt(void) const
{
	return m_dwVtxCnt;
}

_ulong CStaticMesh::Get_Stride(void) const
{
	return m_dwStride;
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
	//Safe_Release(m_pSampleTexture);
	Safe_Release(m_pAdjacency);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubset);
	Safe_Release(m_pOriMesh);


	for (_uint i = 0; i < m_dwSubsetCnt; ++i)
	{
		if (m_ppTextures != nullptr && m_ppTextures[i] != nullptr)
		{
			m_ppTextures[i]->Release();
			m_ppTextures[i] = nullptr;
			Safe_Release(m_ppTextures[i]);
		}
	}
	Safe_Delete_Array(m_ppTextures);
	Safe_Release(m_pSampleTexture);

	if (m_bIsPrototype == true)
		Safe_Delete_Array(m_pVtxPos);

	CMesh::Free();
}
