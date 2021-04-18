matrix			g_matWorld;	 // 상수 테이블
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler		BaseSampler = sampler_state
{
	texture = g_BaseTexture;

	minfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float4			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4			vPosition : POSITION;
	float2			vTexUV : TEXCOORD0;
};

// 정점 쉐이더

VS_OUT			VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};


PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)
	Out.vColor.rg = 0.5f;

	return Out;
}

PS_OUT			PS_TEST(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스처로부터 uv좌표에 해당하는 색을 추출하여 반환하는 함수(반환 타입이 vector타입)
	Out.vColor.r = 0.5f;

	return Out;
}



technique Default_Device
{
	pass	Alphablend	// 기능의 캡슐화
	{
		alphablendenable = true;
		srcblend = srcalpha;
		destblend = invsrcalpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass	AlphaTest
	{
		alphatESTenable = true;
		alphafunc = greater;
		alpharef = 0xc0;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_TEST();
	}

	pass	None
	{

	}

};
