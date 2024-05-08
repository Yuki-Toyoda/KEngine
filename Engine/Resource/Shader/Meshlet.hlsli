struct ConstantData
{
    float32_t4x4 WorldViewProj;
    uint32_t     DrawMeshlets;
};

struct WorldTransform
{
    float32_t4x4 world;
};

struct Material
{
    float32_t4 color;
    uint32_t   enableLighting;
};

struct VertexData
{
    float32_t4 pos;
    float32_t2 texCoord;
    float32_t3 normal;
};

struct Meshlet
{
    uint32_t VertCount;
    uint32_t VertOffset;
    uint32_t PrimCount;
    uint32_t PrimOffset;
};

struct DirectionalLight
{
    float32_t4   color;
    float32_t3   direction;
    float32_t    intensity;
};

struct VertexOutPut
{
    float32_t4 pos      : SV_POSITION0;
    float32_t2 texCoord : TEXCOORD0;
    float32_t3 normal   : NORMAL0;
    float32_t4 color    : COLOR0;
};

ConstantBuffer<ConstantData>     ConstantData : register(b0);
ConstantBuffer<DirectionalLight> light        : register(b1);
ConstantBuffer<WorldTransform>   Transform    : register(b2);
ConstantBuffer<Material>         material     : register(b3);

StructuredBuffer<Meshlet>    Meshlets            : register(t0);
StructuredBuffer<VertexData> Vertices            : register(t1);
ByteAddressBuffer            UniqueVertexIndices : register(t2);
StructuredBuffer<uint32_t>   PrimitiveIndices    : register(t3);

Texture2D<float32_t4> gTexture : register(t4);
SamplerState gSampler : register(s0);

float4 TransformPosition(float4 v)
{
    return mul(mul(v, Transform.world), ConstantData.WorldViewProj);
}