#pragma once
#include "StaticObject.h"
class CBaseItem abstract : public CStaticObject
{
protected:
    explicit CBaseItem(_Device pDevice);
    explicit CBaseItem(const CBaseItem& other);
    virtual ~CBaseItem() = default;

public :
    // CStaticObject을(를) 통해 상속됨
    virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
    virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
    virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
    virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
    virtual HRESULT Render_GameObject(void) PURE;


public:
    _uint Get_Amount();             //얼마만큼 지급할것인지

    virtual CGameObject* Clone(void* pArg = nullptr) PURE;

protected:
    _uint m_iAmount;                //얼마만큼 있는지


    virtual void Free(void);


};

