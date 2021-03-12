#include "DynamicMesh.h"

USING(Engine)

CDynamicMesh::CDynamicMesh(_Device pDevice)
    : CMesh(pDevice)
    , m_pRootFrame(nullptr)
    , m_pLoader(nullptr)
    , m_pAniControl(nullptr)
{
}

CDynamicMesh::CDynamicMesh(const CDynamicMesh& other)
    : CMesh(other)
    , m_pRootFrame(other.m_pRootFrame)
    , m_pLoader(other.m_pLoader)
    , m_MeshContainerList(other.m_MeshContainerList)
    , m_pAniControl(nullptr)
{
    //애니메이션 컨트롤러는 깊은복사로 가져가야됨.
    m_pAniControl = CAnimationController::Create(*other.m_pAniControl);
}

const D3DFRAME_DERIVED* CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
    return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool CDynamicMesh::End_AnimationSet()
{
    return m_pAniControl->End_AnimationSet();
}

//메쉬 로더
HRESULT CDynamicMesh::Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar	szFullPath[256] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pDevice, pFilePath);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	LPD3DXANIMATIONCONTROLLER		pAniCtrl = nullptr;

	if (FAILED(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pDevice,
		m_pLoader,	// hierarchyLoder
		NULL,
		&m_pRootFrame,
		&pAniCtrl)))	// AniCtrl
		return E_FAIL;

	m_pAniControl = CAnimationController::Create(pAniCtrl);
	NULL_CHECK_RETURN(m_pAniControl, E_FAIL);

	Safe_Release(pAniCtrl);


	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void CDynamicMesh::Render_Meshes(void)
{
	for (auto& iter : m_MeshContainerList)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = iter;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffsetMatrix[i] * *pMeshContainer->ppFrameCombinedMatrix[i];
		}

		void* pDestVtx = nullptr;
		void* pSourVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pDestVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pSourVtx);

		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경 또한 동시에 수행)
		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, NULL, pDestVtx, pSourVtx);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pDevice->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}

		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}
}

void CDynamicMesh::Set_AnimationSet(const _uint& iIndex)
{
	if (nullptr == m_pAniControl)
		return;

	m_pAniControl->Set_AnimationSet(iIndex);
}

void CDynamicMesh::Play_AnimationSet(const _float& fDeltatime)
{
	if (nullptr == m_pAniControl)
		return;

	m_pAniControl->Play_AnimationSet(fDeltatime);

	_matrix		matTemp;
	Update_FrameMatrices((D3DXFRAME_DERIVED*)m_pRootFrame, D3DXMatrixIdentity(&matTemp));
}

void CDynamicMesh::Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	//프레임 별 매트릭스 업데이트 
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTranformationMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrices((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTranformationMatrix);
}

void CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED* pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		{
			// 뼈의 이름을 얻어옴
			const char* pBoneName = pDerivedMeshContainer->pSkinInfo->GetBoneName(i);
			// 이름과 일치하는 뼈를 찾음
			D3DXFRAME_DERIVED* pFindFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);

			pDerivedMeshContainer->ppFrameCombinedMatrix[i] = &pFindFrame->CombinedTranformationMatrix;
		}

		m_MeshContainerList.push_back(pDerivedMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);
}

CDynamicMesh* CDynamicMesh::Create(_Device pDevice, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh* pInstance = new CDynamicMesh(pDevice);

	if (FAILED(pInstance->Ready_Meshes(pFilePath, pFileName)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CDynamicMesh::Clone(void* pArg)
{
	return new CDynamicMesh(*this);
}

void CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniControl);

	if (true == m_bIsPrototype)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}

	m_MeshContainerList.clear();
}
