#include "framework.h"
#include "NaviMeshController.h"

HRESULT CNaviMeshController::Compare_NaviMove(std::map<const _tchar*, Engine::CLayer*>* pTargetScene)
{
	for (auto& iter : *pTargetScene)
	{
		if (iter == *pTargetScene->end())
		{
			return E_FAIL;
		}
		if (iter.second == nullptr)
		{
			continue;
		}

		for (auto& targetObject : *(iter.second->Get_ObjectLayer()))
		{
			if (targetObject.second == nullptr)
			{
				continue;
			}
			if (targetObject.second->m_mapComponent)

		}



	}




	return S_OK;
}
