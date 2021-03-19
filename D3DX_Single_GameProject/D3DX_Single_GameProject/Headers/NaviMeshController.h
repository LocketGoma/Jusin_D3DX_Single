#pragma once

#ifndef __NAVIMESH_CONTROLLER_H__
#define __NAVIMESH_CONTROLLER_H__

#include "GameObject.h"
#include "Base.h"
//대충 씬에 탑제되어서 돌아가는 친구
//레이어에 안넣음!

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