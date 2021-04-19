#pragma once
#include "StaticObject.h"
class CBaseItem abstract : public CStaticObject
{
protected:
    explicit CBaseItem(_Device pDevice);
    explicit CBaseItem(const CBaseItem& other);
    virtual ~CBaseItem() = default;

public :
    // CStaticObject��(��) ���� ��ӵ�
    virtual HRESULT Ready_GameObject(_uint iTexNumber = 0) PURE;
    virtual HRESULT Ready_GameObject_Clone(void* pArg) PURE;
    virtual _int Update_GameObject(const _float& fDeltaTime) PURE;
    virtual _int LateUpdate_GameObject(const _float& fDeltaTime) PURE;
    virtual HRESULT Render_GameObject(void) PURE;


public:
    _uint Get_Amount();             //�󸶸�ŭ �����Ұ�����

    virtual CGameObject* Clone(void* pArg = nullptr) PURE;

protected:
    _uint m_iAmount;                //�󸶸�ŭ �ִ���


    virtual void Free(void);


};

