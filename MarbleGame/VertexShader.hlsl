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
    float4 color : COLOR;
    float2 textureUV : TEXTURE;
};

struct PixelInputType
{
    float4 position : POSITION;
    float4 color: COLOR;
    float2 textureUV : TEXTURE;
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

    output.color = input.color;

    return output;
};