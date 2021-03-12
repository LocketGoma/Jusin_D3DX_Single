#pragma once

#ifndef __HIERARCHY_LOADER_H__
#define __HIERARCHY_LOADER_H__

#include "Engine_Include.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CHierarchyLoader : public ID3DXAllocateHierarchy
{
private:
	explicit CHierarchyLoader(_Device pDevice, const _tchar* pPath);
	virtual ~CHierarchyLoader() = default;

public:
	//__stdcall 예약 메시지
	//사실 THIS_ 는 별 의미 없긴 함
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME* ppNewFrame);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR Name, CONST D3DXMESHDATA* pMeshData, CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances, DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo, LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree);

private:
	void	Allocate_Name(char** ppName, const char* pFrameName);

private:
	_Device				m_pDevice;
	const _tchar* m_pPath;

public:
	static CHierarchyLoader* Create(_Device pDevice, const _tchar* pPath);
	_ulong		Release(void);


};

END_NAMESPACE

#endif // !__HIERACHY_LOADER_H__