#include "ControlSupport.h"
#include "VIBuffer.h"
#include "VTXTerrain.h"
#include "Transform.h"

#include "Collider.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"

USING(Engine)

CControlSupportUnit::CControlSupportUnit(_Device pDevice)
    : m_pDevice(pDevice)
{
    m_bIsPrototype = true;

    

    Safe_AddReference(m_pDevice);
}

CControlSupportUnit::CControlSupportUnit(const CControlSupportUnit& other)
    : CComponent(other)
    , m_pDevice(other.m_pDevice)
{
    m_bIsPrototype = false;

    

    Safe_AddReference(m_pDevice);
}

HRESULT CControlSupportUnit::Ready_SupportUnit()
{
    return S_OK;
}

//근데 인터벌이 왜 ulong이지?
_float CControlSupportUnit::Calculate_HeightOnTerrain(const _vec3* pPos, const _vec3* pTerrainVTX, const _ulong& dwCountX, const _ulong& dwCountZ, const _ulong& dwVTXInterval)
{
    //(Z축 현재 위치 / 각 터레인별 간격) * X축 개수 + (X축 현재 위치 / 각 터레인별 간격);
    _ulong dwIndex = _ulong(pPos->z / dwVTXInterval) * dwCountX + _ulong(pPos->x / dwVTXInterval);

    //기준점이 우상단. 우상단에서 좌측으로 가서 -> 현 위치 - 계산된 값
    _float vRatioX = (pPos->x - pTerrainVTX[dwIndex+dwCountX].x)/dwVTXInterval;
    //기준점이 우상단. 우상단에서 아래로 가서 -> 계산된 값 - 현 위치
    _float vRatioZ = (pTerrainVTX[dwIndex+dwCountX].z - pPos->z)/dwVTXInterval;

    //0 : 좌하단
    //1 : 우하단
    //2 : 우상단
    //3 : 좌상단
    _float fHeight[4] = { pTerrainVTX[dwIndex].y,pTerrainVTX[dwIndex + 1].y,pTerrainVTX[dwIndex + dwCountX + 1].y,pTerrainVTX[dwIndex + dwCountX].y, };
    
    _float fResult = fHeight[3];

    //우상단
    if (vRatioX > vRatioZ)
    {
        fResult = fHeight[3] + (fHeight[2] - fHeight[3]) * vRatioX + (fHeight[1] - fHeight[2]) * vRatioZ;
    }
    //좌하단
    if (vRatioX < vRatioZ)
    {
        fResult = fHeight[3] + (fHeight[1] - fHeight[0]) * vRatioX + (fHeight[0] - fHeight[3]) * vRatioZ;

    }

    return fResult;
}

_bool CControlSupportUnit::Picking_Object_Static(HWND hWnd, const CStaticMesh* pMesh, const CTransform* pTransform)
{
    return _vec3();
}

//1. ColliderBox-레이 피킹 처리
//2. 1번 통과 시 아래 코드 작동

//이거 중력건 만들려면 반드시 만들어놔야됨.
_bool CControlSupportUnit::Picking_Object_Dynamic(HWND hWnd, const CDynamicMesh* pMesh, const CTransform* pTransform)
{
    _bool bResult = false;

    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(hWnd, &ptMouse);

    _vec3 vMousePos;

    //뷰포트 획득
    D3DVIEWPORT9 pViewPort;
    ZeroMemory(&pViewPort, sizeof(_D3DVIEWPORT9));
    m_pDevice->GetViewport(&pViewPort);

    //윈도우 좌표 -> 투영 좌표
    vMousePos.x = ptMouse.x / (pViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(pViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    //투영 좌표 -> 뷰 좌표
    _mat matProj;
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, NULL, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    //이 시점에 마우스 좌표는 뷰에 있음.

    //뷰 좌표 -> 월드 좌표
    _mat matView;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);


    //RayPos : 레이 스타트 / RayDir : 레이 방향
    _vec3 vRayPos, vRayDir;

    vRayPos = _vec3(0.f, 0.f, 0.f); //레이 시작 : 화면 중점
    vRayDir = vMousePos - vRayPos; //레이 방향 : 마우스 클릭위치

    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    //월드 -> 로컬
    //사유 : 버텍스 정보는 로컬정보니까.
    _mat matWorld;
    memcpy(&matWorld, pTransform->Get_TransformDescription().matWorld, sizeof(_mat));
    D3DXMatrixInverse(&matWorld, NULL, &matWorld);

    //Coord : 위치 / Normal : 방향
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);


    auto* meshList = pMesh->Get_VertrxInfo();
    for (auto& iter : *meshList)
    {
        if (bResult == true)
        {
            break;
        }
        D3DXMESHCONTAINER_DERIVED* pMeshContainer = iter;
        LPDIRECT3DVERTEXBUFFER9 pVTXBuffer;
        LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
        _uint iIndexNumber = pMeshContainer->MeshData.pMesh->GetNumFaces();
        pMeshContainer->MeshData.pMesh->GetVertexBuffer(&pVTXBuffer);
        pMeshContainer->MeshData.pMesh->GetIndexBuffer(&pIndexBuffer);

        WORD* pIndices;
        D3DVERTEX* pVertices;        

        pIndexBuffer->Lock(0, 0, (void**)&pIndices, 0);
        pVTXBuffer->Lock(0, 0, (void**)&pVertices, 0);

        for (_uint i = 0; i < iIndexNumber; i++)
        {
            _vec3 v0 = pVertices[pIndices[3 * i + 0]].p;
            _vec3 v1 = pVertices[pIndices[3 * i + 1]].p;
            _vec3 v2 = pVertices[pIndices[3 * i + 2]].p;

            _float fU, fV, fDist;
            if (D3DXIntersectTri(&v0, &v1, &v2, &vRayPos, &vRayDir, &fU, &fV, &fDist))
            {
                bResult = true;
                break;
            }
        }
        pVTXBuffer->Unlock();
        pIndexBuffer->Unlock();

        Safe_Release(pVTXBuffer);
        Safe_Release(pIndexBuffer);
    }
    return bResult;
}



_vec3 CControlSupportUnit::Picking_Terrain(HWND hWnd, const CVTXTerrain* pBuffer, const CTransform* pTransform)
{
    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(hWnd, &ptMouse);

    _vec3 vMousePos;

    //뷰포트 획득
    D3DVIEWPORT9 pViewPort;
    ZeroMemory(&pViewPort, sizeof(_D3DVIEWPORT9));
    m_pDevice->GetViewport(&pViewPort);

    //윈도우 좌표 -> 투영 좌표
    vMousePos.x = ptMouse.x / (pViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(pViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    //투영 좌표 -> 뷰 좌표
    _mat matProj;
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, NULL, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    //이 시점에 마우스 좌표는 뷰에 있음.

    //뷰 좌표 -> 월드 좌표
    _mat matView;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);


    //RayPos : 레이 스타트 / RayDir : 레이 방향
    _vec3 vRayPos, vRayDir;
    
    vRayPos = _vec3(0.f, 0.f, 0.f); //레이 시작 : 화면 중점
    vRayDir = vMousePos - vRayPos; //레이 방향 : 마우스 클릭위치

    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    //월드 -> 로컬
    //사유 : 버텍스 정보는 로컬정보니까.
    _mat matWorld;
    memcpy(&matWorld, pTransform->Get_TransformDescription().matWorld, sizeof(_mat));
    D3DXMatrixInverse(&matWorld, NULL, &matWorld);

    //Coord : 위치 / Normal : 방향
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

    //피킹할 타겟 터레인 획득
    _ulong dwVTXCountX = pBuffer->Get_VTXCountX();
    _ulong dwVTXCountZ = pBuffer->Get_VTXCountZ();
    const _vec3* pTerrainVTX = pBuffer->Get_VTXPos();

    _float fU, fV, fDist;
    _ulong dwVTXIndex[3];

    _vec3 vReturnPos = _vec3(0.f, 0.f, 0.f);

    for (_ulong i = 0; i < dwVTXCountZ - 1; i++)
    {
        for (_ulong j = 0; j < dwVTXCountX - 1; j++)
        {
            _ulong dwIndex = i * dwVTXCountX + j;

            //오른쪽 위
            dwVTXIndex[0] = dwIndex + dwVTXCountX;
            dwVTXIndex[1] = dwIndex + dwVTXCountX + 1;
            dwVTXIndex[2] = dwIndex + 1;

            if (D3DXIntersectTri(&pTerrainVTX[dwVTXIndex[1]], &pTerrainVTX[dwVTXIndex[2]], &pTerrainVTX[dwVTXIndex[0]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
            {
                vReturnPos = _vec3(pTerrainVTX[dwVTXIndex[1]] + (pTerrainVTX[dwVTXIndex[2]] - pTerrainVTX[dwVTXIndex[1]])*fU + (pTerrainVTX[dwVTXIndex[0]]- pTerrainVTX[dwVTXIndex[1]])*fV);

                break;
            }

            //왼쪽 아래
            dwVTXIndex[0] = dwIndex + dwVTXCountX;
            dwVTXIndex[1] = dwIndex + 1;
            dwVTXIndex[2] = dwIndex;

            if (D3DXIntersectTri(&pTerrainVTX[dwVTXIndex[2]], &pTerrainVTX[dwVTXIndex[0]], &pTerrainVTX[dwVTXIndex[1]], &vRayPos, &vRayDir, &fU, &fV, &fDist))
            {
                // = v0 + (u*(v1-v0))+(v*(v2-v0));

                vReturnPos = _vec3(pTerrainVTX[dwVTXIndex[2]] + (pTerrainVTX[dwVTXIndex[0]] - pTerrainVTX[dwVTXIndex[2]]) * fU + (pTerrainVTX[dwVTXIndex[1]] - pTerrainVTX[dwVTXIndex[2]]) * fV);

                break;
            }
        }
    }
    if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
    {
        vReturnPos.x = (float)((int)(vReturnPos.x + 0.5f));
        vReturnPos.y = (float)((int)(vReturnPos.y + 0.5f));
        vReturnPos.z = (float)((int)(vReturnPos.z + 0.5f));
    }

       
    //없으면 초기값
    return vReturnPos;
}

_bool CControlSupportUnit::Collision_Picking(HWND hWnd, CCollider* pCollider, CTransform* pTransform)
{
    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(hWnd, &ptMouse);

    _vec3 vMousePos;

    //뷰포트 획득
    D3DVIEWPORT9 pViewPort;
    ZeroMemory(&pViewPort, sizeof(_D3DVIEWPORT9));
    m_pDevice->GetViewport(&pViewPort);

    //윈도우 좌표 -> 투영 좌표
    vMousePos.x = ptMouse.x / (pViewPort.Width * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / -(pViewPort.Height * 0.5f) + 1.f;
    vMousePos.z = 0.f;

    //투영 좌표 -> 뷰 좌표
    _mat matProj;
    m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
    D3DXMatrixInverse(&matProj, NULL, &matProj);
    D3DXVec3TransformCoord(&vMousePos, &vMousePos, &matProj);

    //이 시점에 마우스 좌표는 뷰에 있음.

    //뷰 좌표 -> 월드 좌표
    _mat matView;
    m_pDevice->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matView, NULL, &matView);


    //RayPos : 레이 스타트 / RayDir : 레이 방향
    _vec3 vRayPos, vRayDir;

    vRayPos = _vec3(0.f, 0.f, 0.f); //레이 시작 : 화면 중점
    vRayDir = vMousePos - vRayPos; //레이 방향 : 마우스 클릭위치

    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

    //월드 -> 로컬
    //사유 : 버텍스 정보는 로컬정보니까.
    _mat matWorld;
    memcpy(&matWorld, pTransform->Get_TransformDescription().matWorld, sizeof(_mat));
    D3DXMatrixInverse(&matWorld, NULL, &matWorld);

    //Coord : 위치 / Normal : 방향
    D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matWorld);
    D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matWorld);

    //위까지 마우스 피킹 처리

    _vec3 pDestMin = *(pCollider->Get_Min());
    _vec3 pDestMax = *(pCollider->Get_Max());

    //앞면
    _vec3 pVertex[8];
    pVertex[0] = _vec3(pDestMin.x, pDestMax.y, pDestMin.z);
    pVertex[1] = _vec3(pDestMax.x, pDestMax.y, pDestMin.z);
    pVertex[2] = _vec3(pDestMax.x, pDestMin.y, pDestMin.z);
    pVertex[3] = _vec3(pDestMin.x, pDestMin.y, pDestMin.z);

    // 뒷면
    pVertex[4] = _vec3(pDestMin.x, pDestMax.y, pDestMax.z);
    pVertex[5] = _vec3(pDestMax.x, pDestMax.y, pDestMax.z);
    pVertex[6] = _vec3(pDestMax.x, pDestMin.y, pDestMax.z);
    pVertex[7] = _vec3(pDestMin.x, pDestMin.y, pDestMax.z);

    //정면
    if (D3DXIntersectTri(&pVertex[0], &pVertex[1], &pVertex[2], &vRayPos, &vRayDir, nullptr,nullptr,nullptr))
    {
        return true;
    }
    if (D3DXIntersectTri(&pVertex[1], &pVertex[3], &pVertex[2], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }

    //우측면
    if (D3DXIntersectTri(&pVertex[4], &pVertex[0], &pVertex[3], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }
    if (D3DXIntersectTri(&pVertex[4], &pVertex[3], &pVertex[7], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }

    //우측면
    if (D3DXIntersectTri(&pVertex[1], &pVertex[5], &pVertex[6], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }
    if (D3DXIntersectTri(&pVertex[1], &pVertex[6], &pVertex[2], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }

    //후면
    if (D3DXIntersectTri(&pVertex[4], &pVertex[5], &pVertex[6], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }
    if (D3DXIntersectTri(&pVertex[4], &pVertex[6], &pVertex[7], &vRayPos, &vRayDir, nullptr, nullptr, nullptr))
    {
        return true;
    }


    return false;
}

_bool CControlSupportUnit::Collision_OBB(const _vec3* pDestMin, const _vec3* pDestMax, const _mat* pDestWorld, const _vec3* pSourceMin, const _vec3* pSourceMax, const _mat* pSourceWorld)
{
    return _bool();
}

_bool CControlSupportUnit::Collision_Sphere(const _vec3* pDestPos, const _float fDestRadius, const _vec3* pSourcePos, const _float fSourceRadius)
{
    _vec3 vRangePos = *pDestPos - *pSourcePos;
    _float fRadiusRange = fDestRadius + fSourceRadius;
    _float fSphereRange = D3DXVec3Length(&vRangePos);
    
    return fRadiusRange >= fSphereRange;
}

void CControlSupportUnit::Set_Point(OBB* pObb, const _vec3* pMin, const _vec3* pMax)
{
}

void CControlSupportUnit::Set_Axis(OBB* pObb)
{
}

CControlSupportUnit* CControlSupportUnit::Create(_Device pDevice)
{
    CControlSupportUnit* pInstance = new CControlSupportUnit(pDevice);

    if (FAILED(pInstance->Ready_SupportUnit()))
    {
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CControlSupportUnit::Clone(void* pArg)
{
    return new CControlSupportUnit(*this);
}

void CControlSupportUnit::Free()
{
    Safe_Release(m_pDevice);
}
