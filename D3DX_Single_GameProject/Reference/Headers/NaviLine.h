#pragma once

#ifndef __NAVILINE_H__
#define __NAVILINE_H__

#include "Engine_Include.h"
#include "Base.h"
#include "Cell.h"

BEGIN_NAMESPACE(Engine)

class ENGINE_DLL CNaviLine : public CBase
{
private:
	explicit CNaviLine(void);	
	virtual ~CNaviLine() = default;

public:
	HRESULT		Ready_Line(const _vec2* pPointA, const _vec2* pPointB);
	COMPARE		Compare(const _vec2* pEndPos);

private:
	_vec2				m_vPoint[(_uint)POINTTRACK::POINT_END];
	_vec2				m_vDirection;
	_vec2				m_vNormal;

public:
	static CNaviLine* Create(const _vec2* pPointA, const _vec2* pPointB);
	virtual void		Free(void);

};

END_NAMESPACE


#endif // !__NAVILINE_H__