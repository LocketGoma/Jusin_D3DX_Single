#pragma once

#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include "Mesh.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(_Device pDevice);
	explicit CStaticMesh(const CStaticMesh& other);
	virtual ~CStaticMesh() = default;

public:
	HRESULT Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void Render_Meshes();

public:
	static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void		Free(void);

private:
	LPD3DXMESH				m_pMesh;		// �븻 ������ ���������� ���Ե� �޽� �� ��ü

	LPD3DXBUFFER			m_pAdjacency;		// �����ϴ� ������ ������ �����ϱ� ���� �ּ�(ù ��° �ּ� ����)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// ������� ����


	D3DXMATERIAL* m_pMtrl;			// ���������� �ؽ����� �̸��� �����ϱ� ���� ����ü ������
												// ������� ���� == �ؽ�ó�� ���� == ������ ����
	LPDIRECT3DTEXTURE9* m_ppTextures;		// �������� �ؽ�ó�� �����ϱ� ���� �迭 ������
	LPDIRECT3DTEXTURE9 m_pSampleTexture;						//���� �ؽ���

	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// ������ ũ��(������) ����
	_vec3* m_pVtxPos;			// ���� ���� �� ������ ���� ������ ���� ������


	// �ٿ�� �ڽ��� ���� ���� �۾�, �븻 ������ ���� �޽��� ������ �븻 ���� �����ϱ� ����
	LPD3DXMESH			m_pOriMesh;	// ���� �ε� �ÿ� ���� �޽� �İ�ü

};

END_NAMESPACE

#endif // !__STATIC_MESH_H__