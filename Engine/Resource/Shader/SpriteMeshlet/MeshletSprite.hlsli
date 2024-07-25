#include "../Constant.hlsli"

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<WorldTransform>   Transform    : register(b2);
ConstantBuffer<Material>         material     : register(b3);

StructuredBuffer<Meshlet>    Meshlets            : register(t0);
StructuredBuffer<VertexData> Vertices            : register(t1);
ByteAddressBuffer            UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>   PrimitiveIndices    : register(t3);

Texture2D<float32_t4> gTexture[512] : register(t4);
SamplerState gSampler : register(s0);

float4 TransformPosition(float4 v)
{
    return mul(mul(v, Transform.world), ConstantData.ViewProjection2D);
}