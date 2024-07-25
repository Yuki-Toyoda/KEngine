#include "../Constant.hlsli"

struct Well
{
    float32_t4x4 skeltonSpaceMatrix;
    float32_t4x4 skeltonSpaceInverseTransposeMatrix;
};

struct Skinned
{
    float32_t4 position;
    float32_t3 normal;
};

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<WorldTransform>   Transform    : register(b2);

StructuredBuffer<Meshlet>        Meshlets            : register(t0);
StructuredBuffer<VertexData>     Vertices            : register(t1);
ByteAddressBuffer                UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>       PrimitiveIndices    : register(t3);
StructuredBuffer<Material>       materials           : register(t4);

StructuredBuffer<Well>      gMatrixPalette : register(t5);

Texture2D<float32_t4> gTexture[512] : register(t6);
SamplerState gSampler : register(s0);

float4 TransformPosition(float4 v)
{
    return mul(mul(v, Transform.world), ConstantData.WorldViewProj);
}