#include "Mesh.h"

USING(Engine)

CMesh::CMesh(_Device pDevice)
    : CGraphicResources(pDevice)
{
}

CMesh::CMesh(const CMesh& other)
    :CGraphicResources(other)
{
}

void CMesh::Free(void)
{
    CGraphicResources::Free();
}
