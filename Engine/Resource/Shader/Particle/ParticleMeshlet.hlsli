#include "../Constant.hlsli"
#include "Particle.hlsli"

struct PayLoad
{
    uint32_t groupID;
};

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<InfoData>         infoData     : register(b2);

StructuredBuffer<Meshlet>        Meshlets            : register(t0);
StructuredBuffer<VertexData>     Vertices            : register(t1);
ByteAddressBuffer                UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>       PrimitiveIndices    : register(t3);
StructuredBuffer<Particle>       ParticleData        : register(t4);
StructuredBuffer<Material>       materials           : register(t5);

Texture2D<float32_t4> gTexture[512] : register(t6);
SamplerState gSampler : register(s0);