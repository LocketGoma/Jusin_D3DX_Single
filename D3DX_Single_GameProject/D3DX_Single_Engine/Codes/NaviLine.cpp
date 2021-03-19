#include "NaviLine.h"

USING(Engine)

CNaviLine::CNaviLine(void)
{
}

HRESULT CNaviLine::Ready_Line(const _vec2* pPointA, const _vec2* pPointB)
{
	m_vPoint[(_uint)POINTTRACK::POINT_START] = *pPointA;
	m_vPoint[(_uint)POINTTRACK::POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[(_uint)POINTTRACK::POINT_FINISH] - m_vPoint[(_uint)POINTTRACK::POINT_START];
	m_vNormal = _vec2(m_vDirection.y * -1.f, m_vDirection.x);
	D3DXVec2Normalize(&m_vNormal, &m_vNormal);

	return S_OK;
}

//왼쪽임 : 지나감
//오른쪽임 : 안지나감
//무슨논리지?
COMPARE CNaviLine::Compare(const _vec2* pEndPos)
{
	_vec2	vDest = *pEndPos - m_vPoint[(_uint)POINTTRACK::POINT_START];
	_float fDot = D3DXVec2Dot(D3DXVec2Normalize(&vDest, &vDest), &m_vNormal);

	if (0.f <= fDot)
		return COMPARE::COMPARE_LEFT;
	else
		return COMPARE::COMPARE_RIGHT;
}

CNaviLine* CNaviLine::Create(const _vec2* pPointA, const _vec2* pPointB)
{
	CNaviLine* pInstance = new CNaviLine;

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
	{
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNaviLine::Free(void)
{
}
