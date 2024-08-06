#ifndef Engine_Struct_h__
#define Engine_Struct_h__

//����� �޽� �����ϰ� �ɶ� �ٽ� ����.

BEGIN_NAMESPACE(Engine)

typedef struct D3DVERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	FLOAT tu, tv;

}D3VERTEX;
const DWORD    FVF_D3DVERTEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;



typedef struct tagVertexColor
{
	_vec3		vPosition;
	_ulong		dwColor;

}VTXCOL;

const _ulong	FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

typedef struct tagVertexTex
{
	_vec3		vPosition;		// 12
	_vec3		vNormal;		// 12
	_vec2		vTexUV;			// 8

}VTXTEX;

const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

typedef struct tagVertexCube
{
	_vec3		vPosition;
	_vec3		vTexUV;

}VTXCUBE;
//���� Normal �� �߰��ؾ����� ����?
const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);	// �ؽ��� UV�� FLOAT�� 3���� ������ ǥ���ϰڴٴ� �ɼ�

typedef struct tagVertexCubeColorNormal
{
	_vec3	vPosition;
	_vec3	vNormal;
	_ulong	wColor;
}VTX_CUBE_COLOR_N;

const _ulong	FVF_CUBECOLORN = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0;


typedef struct tagVertex_Screen
{
	_vec4		vPosition;
	_vec2		vTexUV;

}VTXSCREEN;

const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1; // RHW ������ ������ ��ġ ���� ǥ���ϴ� �ڷ���


typedef struct INDEX16
{
	_ushort		_0;
	_ushort		_1;
	_ushort		_2;

}INDEX16;

typedef struct INDEX32
{
	_ulong		_0;
	_ulong		_1;
	_ulong		_2;

}INDEX32;

typedef	struct D3DFRAME_DERIVED : public D3DXFRAME
{
	_matrix			CombinedTranformationMatrix;	// �θ� ���� ������ ���� ��� ���� �����ϱ� ���� ���

}D3DXFRAME_DERIVED;

typedef	struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTexture;

	LPD3DXMESH				pOriMesh;	// ���� �ε� ������ ���¸� ������ �ִ� �޽� ��ü(�Һ�)

	_ulong					dwNumBones;	// �޽��� ���� ��� ���� ������ ����

	_matrix* pFrameOffsetMatrix;
	// �ִϸ��̼� ������ ������ ������ ��� ���� 

	_matrix** ppFrameCombinedMatrix;
	// �� ����ü�� ���� �ִ� CombinedTransformationMatrix�� �ּҰ����� �����ϴ� ������

	_matrix* pRenderingMatrix;
	// ���������� ����� ���� ����� ����
	// pFrameOffsetMatrix * (*ppFrameCombinedMatrix)�� �����

}D3DXMESHCONTAINER_DERIVED;




END_NAMESPACE


#endif // Engine_Struct_h__
