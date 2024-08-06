#pragma once

#ifndef __GRAPHICRESOURCE_MANAGER_H__
#define __GRAPHICRESOURCE_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"

//여기에 그래픽 리소스 헤더 입력
#include "VTXTriColor.h"
#include "VTXRectTexture.h"
#include "VTXTerrain.h"
#include "VTXCubeColor.h"
#include "VTXCubeTexture.h"
#include "VTXBatchTerrain.h"

#include "Texture.h"

#include "StaticMesh.h"
#include "DynamicMesh.h"
#include "NaviMesh.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGraphicResourceManager : public CBase
{
	DECLARE_SINGLETON(CGraphicResourceManager)

private:
	explicit CGraphicResourceManager();
	virtual ~CGraphicResourceManager()=default;

public:
	//컨테이너 공간을 미리 얼마나 할당할것인지 지정.
	HRESULT Reserve_Size(const _uint& wSize);
	//디바이스 정보, 인덱스 정보, 버퍼 태그명, 버퍼 아이디, 사이즈 (X,Y), 버텍스별 간격
	HRESULT Ready_Buffer(_Device pDevice, const _uint& iIndex, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCountX = 129, const _ulong& dwCountZ = 129, const _ulong& dwVTXInterval = 1);
	HRESULT	Ready_Texture(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTYPE eType, const _tchar* pPath, const _uint& iCnt = 1);
	HRESULT Ready_Meshes(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT Ready_Meshes_Force(_Device pDevice, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName);

	CComponent* Clone_Resource(const _uint& iIndex, const _tchar* pResourceTag);

private:
	CGraphicResources* Find_Resources(const _uint& iIndex, const _tchar* pResourceTag);

private:
	typedef std::map<const _tchar*, CGraphicResources*>		MAPRESOURCES;
	//std::map<const _tchar*, CGraphicResources*>* m_pmapResources;
	MAPRESOURCES* m_pmapResources;

	_ulong										m_dwContainerIdx;

public:
	virtual void		Free(void);

};

END_NAMESPACE

#endif // !__GRAPHICRESOURCE_MANAGER_H__
