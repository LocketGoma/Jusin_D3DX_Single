#include "Component.h"

USING(Engine)

CComponent::CComponent(void)
{
}

CComponent::~CComponent(void)
{
}

_int CComponent::Update_Component(const _float& fTimeDelta)
{
	return 0;
}

_int CComponent::LateUpdate_Component(const _float& fTimeDelta)
{
	return 0;
}
