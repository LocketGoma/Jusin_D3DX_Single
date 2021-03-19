#pragma once

#ifndef __NAVIMESH_CONTROLLER_H__
#define __NAVIMESH_CONTROLLER_H__

#include "GameObject.h"
#include "Base.h"
//���� ���� ž���Ǿ ���ư��� ģ��
//���̾ �ȳ���!

BEGIN_NAMESPACE(Engine)
class CNaviMesh;
class CLayer;
END_NAMESPACE

class CNaviMeshController : Engine::CGameObject
{

public:
	void Set_NaviMesh();
	void Set_NaviMesh(const _tchar* meshName);

	HRESULT Compare_NaviMove(std::map<const _tchar*, Engine::CLayer*>* pTargetScene);


private:
	Engine::CNaviMesh* m_pNaviMeshCom = nullptr;

};


#endif // !__NAVIMESH_CONTROLLER_H__