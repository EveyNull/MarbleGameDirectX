cbuffer MatrixBuffer
{
    matrix rotationMatrix;
    matrix translationMatrix;
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 textureUV : TEXCOORD;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureUV : TEXCOORD;
    float3 normal : NORMAL;
};

PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = mul(input.position, rotationMatrix);
    output.position = mul(output.position, translationMatrix);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.normal = mul(input.normal, mul(rotationMatrix, worldMatrix));

    output.textureUV = input.textureUV;

    return output;
};