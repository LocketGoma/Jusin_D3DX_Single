float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

float		g_fEndTimer;

texture		g_OriginalTexture;

sampler		OrginalSampler = sampler_state
{
	texture = g_OriginalTexture;

	minfilter = linear;
	magfilter = linear;

};

texture		g_EndTexture;

sampler		EndSceneSampler = sampler_state
{
	texture = g_EndTexture;

	minfilter = linear;
	magfilter = linear;

};

struct PS_IN
{
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vOriginal = tex2D(OrginalSampler, In.vTexUV);
	vector	vEndScene = tex2D(EndSceneSampler, In.vTexUV);	

	Out.vColor = (vOriginal*(1- g_fEndTimer)) + (vEndScene * g_fEndTimer);

	return Out;
}

technique Default_Device
{
	pass
	{
		//zwriteenable = false;

		//alphablendenable = true;
		//srcblend = srcalpha;
		//destblend = invsrcalpha;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
