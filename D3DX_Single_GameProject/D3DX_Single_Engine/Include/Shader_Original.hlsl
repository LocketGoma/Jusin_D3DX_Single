float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

texture g_OriginalTexture;

sampler OriginalSampler = sampler_state
{
	texture = g_OriginalTexture;

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

	Out.vColor = tex2D(OriginalSampler, In.vTexUV);

	return Out;
}

technique Default_Device
{
	pass
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
