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
	LPD3DXMESH				m_pMesh;		// 노말 정보가 최종적으로 삽입된 메쉬 컴 객체

	LPD3DXBUFFER			m_pAdjacency;		// 인접하는 폴리곤 정보를 보관하기 위한 주소(첫 번째 주소 보관)
	LPD3DXBUFFER			m_pSubset;
	_ulong					m_dwSubsetCnt;		// 서브셋의 개수


	D3DXMATERIAL* m_pMtrl;			// 재질정보와 텍스쳐의 이름을 보관하기 위한 구조체 포인터
												// 서브셋의 개수 == 텍스처의 개수 == 재질의 개수
	LPDIRECT3DTEXTURE9* m_ppTextures;		// 여러장의 텍스처를 보관하기 위한 배열 포인터
	LPDIRECT3DTEXTURE9 m_pSampleTexture;						//샘플 텍스쳐

	_ulong					m_dwVtxCnt;
	_ulong					m_dwStride;			// 정점의 크기(사이즈) 보관
	_vec3* m_pVtxPos;			// 정점 정보 중 포지션 값만 얻어오기 위한 포인터


	// 바운딩 박스와 관계 없는 작업, 노말 정보가 없는 메쉬의 정점에 노말 값을 삽입하기 위함
	LPD3DXMESH			m_pOriMesh;	// 최초 로딩 시에 얻어온 메쉬 컴객체

};

END_NAMESPACE

#endif // !__STATIC_MESH_H__