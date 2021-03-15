#include "HierarchyLoader.h"

USING(Engine)

CHierarchyLoader::CHierarchyLoader(_Device pDevice, const _tchar* pPath)
	:m_pDevice(pDevice)
	, m_pPath(pPath)
{
	Safe_AddReference(m_pDevice);
}

//프레임 제작.
STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	D3DXFRAME_DERIVED* pDerivedFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pDerivedFrame, sizeof(D3DXFRAME_DERIVED));		//???

	Allocate_Name(&pDerivedFrame->Name, Name);

	pDerivedFrame->CombinedTranformationMatrix = *D3DXMatrixIdentity(&pDerivedFrame->TransformationMatrix);

	//만들어진 프레임 리턴.
	*ppNewFrame = pDerivedFrame;

	return S_OK;
}
//사실 여기가 제일 중요해요 메쉬컨테이너 가져오는곳이라
STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::CreateMeshContainer(LPCSTR Name, const D3DXMESHDATA* pMeshData, const D3DXMATERIAL* pMaterials, const D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, const DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED* pDerivedMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pDerivedMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pDerivedMeshContainer->Name, Name);

	pDerivedMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	_ulong dwNumFaces = pMesh->GetNumFaces();	// 메쉬가 지닌 폴리곤의 개수를 반환

	pDerivedMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pDerivedMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	//메쉬로부터 FVF 정보 가져옴.
	_ulong	dwFVF = pMesh->GetFVF();

	// 메쉬의 노말 정보가 없는 경우 코드로 삽입
	if (!(dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(pDerivedMeshContainer->MeshData.pMesh, pDerivedMeshContainer->pAdjacency);
	}
	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pDevice, &pDerivedMeshContainer->MeshData.pMesh);
	}

	pDerivedMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pDerivedMeshContainer->pMaterials = new D3DXMATERIAL[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

	pDerivedMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pDerivedMeshContainer->NumMaterials];
	ZeroMemory(pDerivedMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pDerivedMeshContainer->NumMaterials);

	//머테리얼 정보가 있으면 가져오기
	if (0 != NumMaterials)
	{
		memcpy(pDerivedMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pDerivedMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[256] = L"";
			_tchar		szFileName[256] = L"";

			lstrcpy(szFullPath, m_pPath);

			MultiByteToWideChar(CP_ACP,
				0,
				pDerivedMeshContainer->pMaterials[i].pTextureFilename,
				strlen(pDerivedMeshContainer->pMaterials[i].pTextureFilename),
				szFileName,
				256);

			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, szFullPath, &pDerivedMeshContainer->ppTexture[i])))
				return E_FAIL;
		}
	}
	else
	{
		pDerivedMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pDerivedMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pDerivedMeshContainer->ppTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pDerivedMeshContainer->pSkinInfo = pSkinInfo;
	pDerivedMeshContainer->pSkinInfo->AddRef();


	pDerivedMeshContainer->MeshData.pMesh->CloneMeshFVF(pDerivedMeshContainer->MeshData.pMesh->GetOptions(), dwFVF, m_pDevice, &pDerivedMeshContainer->pOriMesh);

	// 메쉬 컨테이너에 영향을 미치는 뼈의 개수를 반환
	pDerivedMeshContainer->dwNumBones = pDerivedMeshContainer->pSkinInfo->GetNumBones();

	pDerivedMeshContainer->pFrameOffsetMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pFrameOffsetMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->ppFrameCombinedMatrix = new _matrix * [pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pDerivedMeshContainer->dwNumBones);

	pDerivedMeshContainer->pRenderingMatrix = new _matrix[pDerivedMeshContainer->dwNumBones];
	ZeroMemory(pDerivedMeshContainer->pRenderingMatrix, sizeof(_matrix) * pDerivedMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pDerivedMeshContainer->dwNumBones; ++i)
		pDerivedMeshContainer->pFrameOffsetMatrix[i] = *pDerivedMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);

	*ppNewMeshContainer = pDerivedMeshContainer;

	return S_OK;
}

//생성된게 하도 많아서 다 날려줘야됨.
STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree != nullptr)
	{
		Safe_Delete_Array(pFrameToFree->Name);

		if (nullptr != pFrameToFree->pMeshContainer)
			DestroyMeshContainer(pFrameToFree->pMeshContainer);

		if (nullptr != pFrameToFree->pFrameSibling)
			DestroyFrame(pFrameToFree->pFrameSibling);

		if (nullptr != pFrameToFree->pFrameFirstChild)
			DestroyFrame(pFrameToFree->pFrameFirstChild);

		Safe_Delete(pFrameToFree);
	}
	return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CHierarchyLoader::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED* pDerivedMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pDerivedMeshContainer->NumMaterials; ++i)
		Safe_Release(pDerivedMeshContainer->ppTexture[i]);

	Safe_Delete_Array(pDerivedMeshContainer->ppTexture);

	Safe_Delete_Array(pDerivedMeshContainer->pMaterials);
	Safe_Delete_Array(pDerivedMeshContainer->pAdjacency);


	Safe_Release(pDerivedMeshContainer->MeshData.pMesh);
	Safe_Release(pDerivedMeshContainer->pOriMesh);
	Safe_Release(pDerivedMeshContainer->pSkinInfo);

	Safe_Delete_Array(pDerivedMeshContainer->Name);
	Safe_Delete_Array(pDerivedMeshContainer->pFrameOffsetMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pDerivedMeshContainer->pRenderingMatrix);

	Safe_Delete(pDerivedMeshContainer);

	return S_OK;
}

void CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);
}

CHierarchyLoader* CHierarchyLoader::Create(_Device pDevice, const _tchar* pPath)
{
	return new CHierarchyLoader(pDevice, pPath);
}

_ulong CHierarchyLoader::Release(void)
{
	Safe_Release(m_pDevice);

	delete this;

	return NO_ERROR;
}
