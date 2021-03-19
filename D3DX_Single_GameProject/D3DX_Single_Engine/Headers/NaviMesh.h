#pragma once

#ifndef __NAVIMESH_H__
#define __NAVIMESH_H__

#include "Mesh.h"
#include "Cell.h"
BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CNaviMesh : public CMesh
{
private:
	explicit CNaviMesh(_Device pDevice);
	explicit CNaviMesh(const CNaviMesh& other);
	virtual ~CNaviMesh() = default;

public:
	const _ulong* Get_CellIndex();
	void Set_CellIndex(const _ulong& dwIndex);

	HRESULT				Ready_NaviMesh(void);
	void				Render_NaviMesh(void);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_vec3				Compare_OnNaviMesh(const _vec3* pOldPos, const _vec3* pNewPos);

private:
	HRESULT				Link_Cell(void);

private:
	std::vector<CCell*>		m_vecCell;
	_ulong				m_dwCellIndex;

public:
	static CNaviMesh* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;


};

END_NAMESPACE

#endif // !__NAVIMESH_H__