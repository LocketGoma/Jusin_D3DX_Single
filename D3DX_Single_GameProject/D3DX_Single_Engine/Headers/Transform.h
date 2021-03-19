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
		//_float	fRotate, fSpeed;
	}TRANSFORM_DESC;

private:
	explicit CTransform(_Device pDevice);
	explicit CTransform(const CTransform& other);
	virtual ~CTransform() = default;


public:
	HRESULT Ready_Transform();
	virtual _int Update_Component(const _float& fDeltaTime = 0.f);
	virtual _int LateUpdate_Component(const _float& fDeltaTime = 0.f);

//Control + Setter
public:
	void Move_Pos(const _vec3* pDir, const _float& fSpeed, _float fDeltaTime);	
	void Rotation(ROTATION eType, const _float& fAngle);
	void Set_Pos(_vec3 vPos);
	void Set_Scale(_vec3 vScale);
	void Set_TransformDescription(TRANSFORM_DESC* pDesc);


//Getter
public :
	TRANSFORM_DESC Get_TransformDescription() const;
	void Get_Info(TRANSFORM_INFO eType, _vec3* pvInfo) const;
	_vec3 Get_Info(TRANSFORM_INFO eType) const;
	_vec3 Get_Info_RawData(TRANSFORM_INFO eType) const;


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

	_Device m_pDevice;
};

END_NAMESPACE

#endif // !__TRANSFROM_H__