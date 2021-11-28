Texture2D shaderTexture;
SamplerState SampleType;

SamplerState SkyBoxSampler
{
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
	AddressU = Mirror;
	AddressV = Mirror;
};

cbuffer SkyboxBuffer
{
	matrix translationMatrix;
	matrix viewBuffer;
	matrix projectionBuffer;
};

struct SkyboxVertexInput
{
	float4 position : POSITION;
	float2 textureUV : TEXCOORD;
};

struct SkyboxPixelInput
{
	float4 position : SV_POSITION;
	float2 textureUV : TEXCOORD;
};

SkyboxPixelInput SkyboxVertexShader(SkyboxVertexInput input)
{
	SkyboxPixelInput output;


	matrix WVP = mul(mul(translationMatrix, viewBuffer), projectionBuffer);

	output.position = mul(input.position, WVP);

	output.textureUV = input.textureUV;

	return output;
}

float4 SkyboxPixelShader(SkyboxPixelInput input) : SV_TARGET
{
	return shaderTexture.Sample(SkyBoxSampler, input.textureUV);
}