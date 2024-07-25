#include "../Constant.hlsli"

struct InstanceData
{
    int32_t instanceCount;
};

struct PayLoad
{
    uint32_t groupID;
};

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<InstanceData>     instanceData : register(b2);

StructuredBuffer<Meshlet>        Meshlets            : register(t0);
StructuredBuffer<VertexData>     Vertices            : register(t1);
ByteAddressBuffer                UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>       PrimitiveIndices    : register(t3);
StructuredBuffer<WorldTransform> Transforms          : register(t4);
StructuredBuffer<Material>       materials           : register(t5);

Texture2D<float32_t4> gTexture[512] : register(t6);
SamplerState gSampler : register(s0);

float4 TransformPosition(float4 v, int32_t gid)
{
    return mul(mul(v, Transforms[gid].world), ConstantData.WorldViewProj);
}