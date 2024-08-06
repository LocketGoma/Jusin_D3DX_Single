#pragma once

#ifndef __NAVIMESH_H__
#define __NAVIMESH_H__

#include "Mesh.h"
#include "Cell.h"
BEGIN_NAMESPACE(Engine)

using namespace std;		//�ƾ� ����� �ڵ�

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
	HRESULT				Load_NaviMesh(const _tchar* pFileName);
	void				Render_NaviMesh(_bool bState = true);
	_vec3				Move_OnNaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);
	_vec3				Compare_OnNaviMesh(const _vec3* pOldPos, const _vec3* pNewPos);
	_vec3				Compare_OnNaviMesh_for_Mesh(const _vec3* pOldPos, const _vec3* pNewPos, const _vec3* pTargetPos);
	_vec3				Caculate_OnNaviMesh(const _vec3* pTargetPos);						//�׺�Ž� Y�� Ÿ�� ������ִ� �Լ�.

	_uint				Caculate_Index(const _vec3* pTargetPos);

	//Get/Set - �׺�Ž� ����
public:
	HRESULT				Add_NaviCell(_vec3& p1, _vec3& p2, _vec3& p3);

	vector<CCell*>*		Get_NaviMesh();									//���̺��
	HRESULT				Set_NaviMesh(std::vector<CCell*>* vCellList);	//�ε��

	HRESULT				Edit_NaviMesh(_uint iIndex, _vec3& p1, _vec3& p2, _vec3& p3);
	HRESULT				Delete_NaviMesh(_uint iIndex);

	HRESULT				Clear_NaviMesh();								//�׺�Ž� �ʱ�ȭ

private:
	HRESULT				Link_Cell(void);

private:
	vector<CCell*>		m_vecCell;
	_ulong				m_dwCellIndex;

public:
	static CNaviMesh* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free(void) override;


};

END_NAMESPACE

#endif // !__NAVIMESH_H__