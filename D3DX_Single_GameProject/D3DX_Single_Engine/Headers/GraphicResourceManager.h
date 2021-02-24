#pragma once

#ifndef __GRAPHICRESOURCE_MANAGER_H__
#define __GRAPHICRESOURCE_MANAGER_H__

#include "Engine_Include.h"
#include "Base.h"

//���⿡ �׷��� ���ҽ� ��� �Է�
#include "VTXTriColor.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CGraphicResourceManager : public CBase
{
	DECLARE_SINGLETON(CGraphicResourceManager)

private:
	explicit CGraphicResourceManager();
	virtual ~CGraphicResourceManager()=default;

public:
	//�����̳� ������ �̸� �󸶳� �Ҵ��Ұ����� ����.
	HRESULT Reserve_Size(const _uint& wSize);
	//����̽� ����, �ε��� ����, ���� �±׸�, ���� ���̵�, ������ (X,Y), ���ؽ��� ����
	HRESULT Ready_Buffer(_Device pDevice, const _uint& iIndex, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCountX = 129, const _ulong& dwCountZ = 129, const _ulong& dwVTXInterval = 1);
	void Render_Buffer(const _uint& iIndex, const _tchar* pBufferTag);

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
