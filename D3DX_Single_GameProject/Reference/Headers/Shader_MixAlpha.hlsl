float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

matrix			g_matWorld;	 // ��� ���̺�
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

vector		g_vLightDir;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlDiffuse;
vector		g_vMtrlAmbient;
vector		g_vMtrlSpecular;

float		g_fPower;

vector		g_vCamPos;


struct VS_IN
{
	float4			vPosition : POSITION;
	float4			vNormal		: NORMAL;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4			vPosition	: POSITION;
	float4			vNormal		 : NORMAL;
	float2			vTexUV		: TEXCOORD0;
	float4			vProjPos : TEXCOORD1;
};

// ���� ���̴�

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;


	return Out;
}

struct PS_IN
{
	vector			vNormal : NORMAL;
	float2			vTexUV : TEXCOORD0;
	float4			vProjPos : TEXCOORD1;

};

struct PS_OUT
{
	vector		vColor : COLOR0;
	//vector		vNormal : COLOR1;
	//vector		vDepth : COLOR2;
};

PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vColor *= saturate((g_vLightDiffuse * g_vMtrlDiffuse) + (g_vLightAmbient * g_vMtrlAmbient) + (g_vLightSpecular * g_vMtrlSpecular));

	if (Out.vColor.r < 0.01 && Out.vColor.g < 0.01 && Out.vColor.b < 0.01)
	{
		Out.vColor.a = 0.0;
	}

	return Out;
}


technique Default_Device
{
	pass	Alphablend	// ����� ĸ��ȭ
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}


};
