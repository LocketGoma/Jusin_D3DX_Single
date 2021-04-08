#include "Component.h"

USING(Engine)

CComponent::CComponent(void)
	: m_bIsPrototype (true)		//최초 생성시에는 true, 복사시 false
{
}

CComponent::CComponent(const CComponent& other)
	: m_bIsPrototype(false)
{
}

CComponent::~CComponent(void)
{
}

_int CComponent::Update_Component(const _float& fDeltaTime)
{
	return 0;
}

_int CComponent::LateUpdate_Component(const _float& fDeltaTime)
{
	return 0;
}
