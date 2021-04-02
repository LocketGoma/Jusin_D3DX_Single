#pragma once

#ifndef __DYNAMIC_MESH_H__
#define __DYNAMIC_MESH_H__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AnimationController.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(_Device pDevice);
	explicit CDynamicMesh(const CDynamicMesh& other);
	virtual ~CDynamicMesh() = default;

public:
	const D3DFRAME_DERIVED* Get_FrameByName(const char* pFrameName);
	_bool	End_AnimationSet();

public:
	HRESULT		Ready_Meshes(const _tchar* pFilePath, const _tchar* pFileName);
	void		Update_Meshes(void);
	void		Render_Meshes(void);

	void		Set_AnimationSet(const _uint& iIndex);
	void		Play_AnimationSet(const _float& fDeltatime);
	_uint		Get_NowAnimationNumber();

public:
	CAnimationController* Get_AniController();
	int Get_VertexCount();
	const std::list<D3DXMESHCONTAINER_DERIVED*>* Get_VertrxInfo() const;

private:
	//  ��� ���� ��ȸ�ϸ鼭 ������ ������ CombinedTransformationMatrix�� �����ϴ� �Լ�
	void		Update_FrameMatrices(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void		SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;		//�ֻ�� ������
	CHierarchyLoader*		m_pLoader;			//����ȭ �δ�
	CAnimationController*	m_pAniControl;		//�ִϸ��̼� ��Ʈ�ѷ�
	int m_iVertexNumber;

	std::list<D3DXMESHCONTAINER_DERIVED*>			m_MeshContainerList;

public:
	static CDynamicMesh* Create(_Device pDevice, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void		Free(void);
};

END_NAMESPACE

#endif // !__DYNAMIC_MESH_H__