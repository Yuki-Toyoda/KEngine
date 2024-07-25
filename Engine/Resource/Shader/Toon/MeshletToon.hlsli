#include "../Constant.hlsli"

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<WorldTransform>   Transform    : register(b2);

StructuredBuffer<Meshlet>    Meshlets            : register(t0);
StructuredBuffer<VertexData> Vertices            : register(t1);
ByteAddressBuffer            UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>   PrimitiveIndices    : register(t3);
StructuredBuffer<Material>   materials           : register(t4);

Texture2D<float32_t4> gTexture[512] : register(t5);
SamplerState gSampler : register(s0);
SamplerState toonMapSampler : register(s1);

float4 TransformPosition(float4 v)
{
    return mul(mul(v, Transform.world), ConstantData.WorldViewProj);
}