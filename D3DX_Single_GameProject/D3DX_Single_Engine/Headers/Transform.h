#pragma once

#ifndef __TRANSFROM_H__
#define __TRANSFROM_H__

#include "Engine_Include.h"
#include "Component.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CTransform : public CComponent
{
public:
	typedef struct tagTransformDescription
	{
		_vec3	m_vInfo[(_uint)(TRANSFORM_INFO::INFO_END)];
		_vec3	vScale, vRotate;
		_mat	matWorld;
		_float	fSpeed, fRotate;

	}TRANSFORM_DESC;

private:
	explicit CTransform();
	explicit CTransform(const CTransform& other);
	virtual ~CTransform() = default;


public:
	HRESULT Ready_Transform();
	virtual _int Update_Component(const _float& fDeltaTime);

//Control
public:
	void Go_Straight(_float fDeltaTime);
	void Go_Side(_float fDeltaTime);


//Get/Set
public :
	TRANSFORM_DESC Get_TransformDescription();

//make & clone
public:
	static CTransform* Create(_Device pDevice);
	virtual CComponent* Clone(void* pArg = nullptr) override;
	virtual void Free() override;

private:
	//_vec3 m_vInfo[(_uint)(TRANSFORM_INFO::INFO_END)];
	//_vec3 m_vScale;
	//_vec3 m_vAngle;
	//_mat  m_matWorld;

	TRANSFORM_DESC	m_TransformDesc;
};

END_NAMESPACE

#endif // !__TRANSFROM_H__