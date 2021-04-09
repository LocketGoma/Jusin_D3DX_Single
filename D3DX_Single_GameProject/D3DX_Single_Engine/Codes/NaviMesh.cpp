#include "Engine_Include.h"
#include "NaviMesh.h"

USING(Engine)

CNaviMesh::CNaviMesh(_Device pDevice)
	: CMesh(pDevice)
	, m_dwCellIndex(0)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh& other)
	: CMesh(other)
	, m_vecCell(other.m_vecCell)
	, m_dwCellIndex(other.m_dwCellIndex)
{
	for (auto& iter : m_vecCell)
		iter->AddRef();
}

const _ulong* CNaviMesh::Get_CellIndex()
{
	return &m_dwCellIndex;
}

void CNaviMesh::Set_CellIndex(const _ulong& dwIndex)
{
	m_dwCellIndex = dwIndex;
}

//해줘야 할것 : 여기를 이용해서 네비매시 추가하기.
HRESULT CNaviMesh::Ready_NaviMesh(void)
{
	m_vecCell.reserve(5);

	CCell* pCell = nullptr;

	// 0
	pCell = CCell::Create(m_pDevice, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f), &_vec3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 1
	pCell = CCell::Create(m_pDevice, m_vecCell.size(), &_vec3(0.f, 0.f, 2.f), &_vec3(2.f, 0.f, 2.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 2
	pCell = CCell::Create(m_pDevice, m_vecCell.size(), &_vec3(0.f, 0.f, 4.f), &_vec3(2.f, 0.f, 2.f), &_vec3(0.f, 0.f, 2.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	// 3
	pCell = CCell::Create(m_pDevice, m_vecCell.size(), &_vec3(2.f, 0.f, 2.f), &_vec3(4.f, 0.f, 0.f), &_vec3(2.f, 0.f, 0.f));
	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	if (FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNaviMesh::Load_NaviMesh(const _tchar* pFileName)
{



	if (FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;
}

void CNaviMesh::Render_NaviMesh(void)
{
	for (auto& iter : m_vecCell)
		iter->Render_Cell();
}

_vec3 CNaviMesh::Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir)
{
	_vec3	vEndPos = *pTargetPos + *pTargetDir;

	if (COMPAREMOVE::MOVE == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	{
		return vEndPos;
	}
	else if (COMPAREMOVE::STOP == m_vecCell[m_dwCellIndex]->Compare(&vEndPos, &m_dwCellIndex))
	{
		return *pTargetPos;
	}

	return vEndPos;
}

_vec3 CNaviMesh::Compare_OnNaviMesh(const _vec3* pOldPos, const _vec3* pNewPos)
{
	if (COMPAREMOVE::MOVE == m_vecCell[m_dwCellIndex]->Compare(pNewPos, &m_dwCellIndex))
	{
		return Caculate_OnNaviMesh(pNewPos);
	}
	else if (COMPAREMOVE::STOP == m_vecCell[m_dwCellIndex]->Compare(pNewPos, &m_dwCellIndex))
	{
		return *pOldPos;
	}

	return *pOldPos;
}

//1. 검사할 pos값이 넘어간 경우 -> 보정된 pos 값 리턴
//2. 검사할 pos값이 넘어가진 않은 경우 -> 현재 pos값 리턴
_vec3 CNaviMesh::Compare_OnNaviMesh_for_Mesh(const _vec3* pOldPos, const _vec3* pNewPos, const _vec3* pTargetPos)
{
	if (COMPAREMOVE::STOP == m_vecCell[m_dwCellIndex]->Compare(pTargetPos, &m_dwCellIndex))
	{
		return Caculate_OnNaviMesh(pNewPos);
	}
	else
	{
		return Caculate_OnNaviMesh(pOldPos);
	}
}

_vec3 CNaviMesh::Caculate_OnNaviMesh(const _vec3* pTargetPos)
{
	return _vec3(pTargetPos->x, m_vecCell[m_dwCellIndex]->Get_Height_At_Cell(pTargetPos),pTargetPos->z);
}

HRESULT CNaviMesh::Add_NaviCell(_vec3& p1, _vec3& p2, _vec3& p3)
{
	CCell* pCell = nullptr;
	pCell = CCell::Create(m_pDevice, m_vecCell.size(), &p1, &p2, &p3);

	NULL_CHECK_RETURN(pCell, E_FAIL);
	m_vecCell.push_back(pCell);

	if (FAILED(Link_Cell()))
		return E_FAIL;

	return S_OK;
}

//세이브용
vector<CCell*>* CNaviMesh::Get_NaviMesh()
{
	return &m_vecCell;
}

HRESULT CNaviMesh::Set_NaviMesh(std::vector<CCell*>* vCellList)
{

	m_vecCell.reserve(vCellList->size());

	for (CCell* cell : *vCellList)
	{
		m_vecCell.push_back(cell);
	}


	return S_OK;
}

HRESULT CNaviMesh::Edit_NaviMesh(_uint iIndex, _vec3& p1, _vec3& p2, _vec3& p3)
{
	if (iIndex >= m_vecCell.size())
	{
		return E_FAIL;
	}
	
	m_vecCell[iIndex]->Edit_Cell(p1, p2, p3);

	return S_OK;
}

HRESULT CNaviMesh::Delete_NaviMesh(_uint iIndex)
{
	if (iIndex > m_vecCell.size())
	{
		return E_FAIL;
	}
	auto iter = m_vecCell.begin();

	for (int i = 0; i < m_vecCell.size(); i++)
	{
		if (i == iIndex)
		{
			Safe_Release(m_vecCell[i]);
			m_vecCell.erase(iter);
			break;
		}
		iter++;
	}
	return S_OK;
}

HRESULT CNaviMesh::Clear_NaviMesh()
{
	for (auto& object : m_vecCell)
	{
		Safe_Release(object);
	}

	m_vecCell.clear();
	m_vecCell.shrink_to_fit();

	if (!m_vecCell.empty())
		return E_FAIL;

	return S_OK;
	
}

//만들어진 셀들 링크하는곳.
HRESULT CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)
	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			// 이웃하는 셀 정보가 없는 경우, 내가 찾은 두개의 정점이 이웃 셀로서 일치하는 경우
			if (nullptr == m_vecCell[i]->Get_Neighbor(NEIGHBOR::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(NAVIPOINT::POINT_A),
					m_vecCell[i]->Get_Point(NAVIPOINT::POINT_B),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(NEIGHBOR::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(NEIGHBOR::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(NAVIPOINT::POINT_B),
					m_vecCell[i]->Get_Point(NAVIPOINT::POINT_C),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(NEIGHBOR::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}


			if (nullptr == m_vecCell[i]->Get_Neighbor(NEIGHBOR::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(NAVIPOINT::POINT_C),
					m_vecCell[i]->Get_Point(NAVIPOINT::POINT_A),
					m_vecCell[i]))
			{
				m_vecCell[i]->Set_Neighbor(NEIGHBOR::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}

		}
	}
	return S_OK;
}

CNaviMesh* CNaviMesh::Create(_Device pDevice)
{
	CNaviMesh* pInstance = new CNaviMesh(pDevice);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CNaviMesh::Clone(void* pArg)
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Free(void)
{
	CMesh::Free();

	//for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteMap());
	Clear_NaviMesh();


}
