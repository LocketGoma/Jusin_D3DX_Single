#include "Cell.h"

#include "NaviLine.h"

USING(Engine)

CCell::CCell(_Device pDevice)
	: m_pDevice(pDevice)
	, m_pD3DXLine(nullptr)
{
	ZeroMemory(m_pNeighbor, sizeof(CCell*) * (_uint)NEIGHBOR::NEIGHBOR_END);
	ZeroMemory(m_pLine, sizeof(CNaviLine*) * (_uint)LINE::LINE_END);

	m_pDevice->AddRef();
}

CCell::CCell(const CCell& other)
	: m_pDevice(other.m_pDevice)
	, m_pD3DXLine(other.m_pD3DXLine)
	, m_dwCellIndex(other.m_dwCellIndex)
{
	memcpy(&m_pNeighbor,other.m_pNeighbor,sizeof(CCell*)* (_uint)NEIGHBOR::NEIGHBOR_END);
	memcpy(&m_pLine,other.m_pLine,sizeof(CNaviLine*)* (_uint)LINE::LINE_END);

	m_pDevice->AddRef();
}

const _vec3* CCell::Get_Point(NAVIPOINT eType)
{
	return &m_vPoint[(_uint)eType];
}
CCell* CCell::Get_Neighbor(NEIGHBOR eType)
{
	return m_pNeighbor[(_uint)eType];
}
const _ulong* CCell::Get_CellIndex(void)
{
	return &m_dwCellIndex;
}
void CCell::Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor)
{
	m_pNeighbor[(_uint)eType] = pNeighbor;
}

HRESULT CCell::Ready_Cell(const _ulong& dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	m_dwCellIndex = dwCellIndex;

	m_vPoint[(_uint)NAVIPOINT::POINT_A] = *pPointA;
	m_vPoint[(_uint)NAVIPOINT::POINT_B] = *pPointB;
	m_vPoint[(_uint)NAVIPOINT::POINT_C] = *pPointC;

	m_pLine[(_uint)LINE::LINE_AB] = CNaviLine::Create(&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_A].x, m_vPoint[(_uint)NAVIPOINT::POINT_A].z),
		&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_B].x, m_vPoint[(_uint)NAVIPOINT::POINT_B].z));

	m_pLine[(_uint)LINE::LINE_BC] = CNaviLine::Create(&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_B].x, m_vPoint[(_uint)NAVIPOINT::POINT_B].z),
		&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_C].x, m_vPoint[(_uint)NAVIPOINT::POINT_C].z));

	m_pLine[(_uint)LINE::LINE_CA] = CNaviLine::Create(&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_C].x, m_vPoint[(_uint)NAVIPOINT::POINT_C].z),
		&_vec2(m_vPoint[(_uint)NAVIPOINT::POINT_A].x, m_vPoint[(_uint)NAVIPOINT::POINT_A].z));

#ifdef _DEBUG
	if (FAILED(D3DXCreateLine(m_pDevice, &m_pD3DXLine)))
		return E_FAIL;
#endif

	return S_OK;
}

_bool CCell::Compare_Point(const _vec3* pFirst, const _vec3* pSecond, CCell* pCell)
{
	if (m_vPoint[(_uint)NAVIPOINT::POINT_A] == *pFirst)
	{
		if (m_vPoint[(_uint)NAVIPOINT::POINT_B] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_AB] = pCell;
			return true;
		}

		if (m_vPoint[(_uint)NAVIPOINT::POINT_C] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	if (m_vPoint[(_uint)NAVIPOINT::POINT_B] == *pFirst)
	{
		if (m_vPoint[(_uint)NAVIPOINT::POINT_C] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_BC] = pCell;
			return true;
		}

		if (m_vPoint[(_uint)NAVIPOINT::POINT_A] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_AB] = pCell;
			return true;
		}
	}

	if (m_vPoint[(_uint)NAVIPOINT::POINT_C] == *pFirst)
	{
		if (m_vPoint[(_uint)NAVIPOINT::POINT_B] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_BC] = pCell;
			return true;
		}

		if (m_vPoint[(_uint)NAVIPOINT::POINT_A] == *pSecond)
		{
			m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_CA] = pCell;
			return true;
		}
	}

	return false;
}

void CCell::Render_Cell(void)
{
	_vec3		vPoint[4];

	vPoint[0] = m_vPoint[(_uint)NAVIPOINT::POINT_A];
	vPoint[1] = m_vPoint[(_uint)NAVIPOINT::POINT_B];
	vPoint[2] = m_vPoint[(_uint)NAVIPOINT::POINT_C];
	vPoint[3] = m_vPoint[(_uint)NAVIPOINT::POINT_A];


	_matrix	matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	for (_ulong i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if (vPoint[i].z <= 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	m_pD3DXLine->SetWidth(3.f); // 선의 굵기를 결정

	m_pDevice->EndScene();
	m_pDevice->BeginScene();

	m_pD3DXLine->Begin();

	//3d 라인을 그려주는 함수
	_matrix		matTemp;
	m_pD3DXLine->DrawTransform(vPoint, 4, D3DXMatrixIdentity(&matTemp), D3DXCOLOR(1.f, 0.f, 0.f, 1.f));

	m_pD3DXLine->End();
}

COMPAREMOVE CCell::Compare(const _vec3* pEndPos, _ulong* pCellIndex)
{
	for (_ulong i = 0; i < (_uint)LINE::LINE_END; ++i)
	{
		if (COMPARE::COMPARE_LEFT == m_pLine[i]->Compare(&_vec2(pEndPos->x, pEndPos->z)))
		{
			if (nullptr == m_pNeighbor[i])
				return COMPAREMOVE::STOP;

			else
			{
				*pCellIndex = *m_pNeighbor[i]->Get_CellIndex();
				return COMPAREMOVE::MOVE;
			}
		}
	}

	return COMPAREMOVE::MOVE;
}

CCell* CCell::Create(_Device pDevice, const _ulong& dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC)
{
	CCell* pInstance = new CCell(pDevice);

	if (FAILED(pInstance->Ready_Cell(dwCellIndex, pPointA, pPointB, pPointC)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free(void)
{
	for (_uint i = 0; i < (_uint)LINE::LINE_END; ++i)
		Safe_Release(m_pLine[i]);

	Safe_Release(m_pD3DXLine);
	Safe_Release(m_pDevice);
}
