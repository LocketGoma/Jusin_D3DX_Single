#pragma once

#ifndef __NAVI_CELL_H__
#define __NAVI_CELL_H__

#include "Engine_Include.h"
#include "Base.h"


BEGIN_NAMESPACE(Engine)

enum class NAVIPOINT { POINT_A, POINT_B, POINT_C, POINT_END };
enum class NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END};
enum class LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
enum class COMPAREMOVE { MOVE, STOP };

class CNaviLine;

class ENGINE_DLL CCell: public CBase
{
private:
	explicit CCell(_Device pDevice);
	explicit CCell(const CCell& otehr);
	virtual ~CCell() = default;

public:
	const _vec3* Get_Point(NAVIPOINT eType);
	CCell* Get_Neighbor(NEIGHBOR eType);
	const _ulong* Get_CellIndex(void);
	void	Set_Neighbor(NEIGHBOR eType, CCell* pNeighbor);


public:
	HRESULT		Ready_Cell(const _ulong& dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	_bool		Compare_Point(const _vec3* pFirst, const _vec3* pSecond, CCell* pCell);
	HRESULT		Edit_Cell(_vec3& p1, _vec3& p2, _vec3& p3);
	void		Render_Cell(void);
	_float		Get_Height_At_Cell(const _vec3* pVPos);

	COMPAREMOVE	Compare(const _vec3* pEndPos, _ulong* pCellIndex);

	

private:
	_vec3		m_vPoint[(_uint)NAVIPOINT::POINT_END];
	CCell*		m_pNeighbor[(_uint)NEIGHBOR::NEIGHBOR_END];
	CNaviLine*	m_pLine[(_uint)LINE::LINE_END];
	_Device		m_pDevice;
	LPD3DXLINE	m_pD3DXLine;
	_ulong		m_dwCellIndex;

	LPD3DXMESH	m_pSphere[(_uint)NAVIPOINT::POINT_END];
	_float		m_fRadius;
	_uint		m_fSlice;

	LPDIRECT3DTEXTURE9 m_pTexture;						//On / Off하면 2개로 늘리기

public:
	static CCell* Create(_Device pDevice, const _ulong& dwCellIndex, const _vec3* pPointA, const _vec3* pPointB, const _vec3* pPointC);
	virtual void		Free(void);


};

END_NAMESPACE

#endif // !__NAVI_CELL_H__