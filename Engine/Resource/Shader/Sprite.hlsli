// 
cbuffer constBuffer : register(b0)
{
    float4 color;
    float4x4 mat;
}

// 
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};
//
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
};

// 
struct Material
{
    // 
    float4 color;
};
// 
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};