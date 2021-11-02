
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer PixelBuffer
{
    float3 lightDir;
    float4 lightColor;
};

struct PixelInputType
{
    float4 position : POSITION;
    float2 textureUV : TEXCOORD;
    float3 normal : NORMAL;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    float4 outputTex = shaderTexture.Sample(SampleType, input.textureUV);

    float3 direction = -lightDir;
    float brightness = saturate(dot(input.normal, direction));

    brightness = clamp(brightness, 0.2, 1);

    float4 color = saturate(lightColor * brightness);
    outputTex = color * outputTex;

    return outputTex;
}