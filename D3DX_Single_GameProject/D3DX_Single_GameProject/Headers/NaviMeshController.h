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
class CScene;
END_NAMESPACE

class CNaviMeshController : public Engine::CGameObject
{
private:
	explicit CNaviMeshController(_Device pDevice);
	explicit CNaviMeshController(const CNaviMeshController& other);
	virtual ~CNaviMeshController() = default;

//�ؿ��� ���ư� �� ����
public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual HRESULT Ready_GameObject_Clone(void* pArg = nullptr) override;
	virtual _int Update_GameObject(const _float& fDeltaTime) override;
	virtual _int LateUpdate_GameObject(const _float& fDeltaTime) override;
	virtual HRESULT Render_GameObject(void) override;

//Get/Set
public:
	_uint Set_NaviMesh(const _tchar* meshName);				
	HRESULT Set_NaviMesh_From_File(const _tchar* pFilePath);
	void Set_NowScene(Engine::CScene* pScene);


//���� + IO
public:
	//�׺�Ž� �����. �� ���� �־��ָ� �̰� ������
	HRESULT Compare_NaviMove(std::map<const _tchar*, Engine::CLayer*>* pTargetScene);
	void Clear_NaviMesh();

public:


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