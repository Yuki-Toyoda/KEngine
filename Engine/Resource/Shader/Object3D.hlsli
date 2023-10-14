cbuffer constBuffer : register(b0)
{
    float4 color;
    float4x4 mat;
    float4x4 world;
    int enableLighting;
    float4x4 uvTransform;
}

// 
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0; 
};
// 
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

// 
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// 
static const int kDirectionalLight = 1;
// 
struct DirectionalLight
{
    float4 color; // 
    float3 direciton; // 
    float intensity; //
    uint active; // 
};

cbuffer LightGroup : register(b1)
{
    DirectionalLight directionalLight[kDirectionalLight];
}