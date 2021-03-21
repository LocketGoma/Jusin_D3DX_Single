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
class CScene;
END_NAMESPACE

class CNaviMeshController : public Engine::CGameObject
{
private:
	explicit CNaviMeshController(_Device pDevice);
	explicit CNaviMeshController(const CNaviMeshController& other);
	virtual ~CNaviMeshController() = default;

//밑에건 돌아갈 일 없음
public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

public:
	_uint Set_NaviMesh(const _tchar* meshName);

	void Set_NowScene(Engine::CScene* pScene);
	HRESULT Compare_NaviMove(std::map<const _tchar*, Engine::CLayer*>* pTargetScene);

public:
	virtual void Set_Position(_vec3 vPos) override;
	virtual void Set_Size(_vec3 vSize) override;
	virtual _vec3 Get_Position() override;
	virtual _vec3 Get_Size() override;

	void Set_Visuable(_bool bState);

private:
	HRESULT			Add_Component(void);

private:
	Engine::CNaviMesh* m_pNaviMeshCom = nullptr;
	Engine::CScene* m_pScene = nullptr;
	_bool m_bVisuable = false;

public:
	static CNaviMeshController* Create(_Device pDevice);
	virtual CGameObject* Clone(void* pArg = nullptr) override;

private:
	virtual void Free(void) override;


};


#endif // !__NAVIMESH_CONTROLLER_H__