
Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : POSITION;
    float2 textureUV : TEXCOORD;
};

float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.textureUV);
}
