struct ConstantData
{
    float32_t4x4 WorldViewProj;
    float32_t4x4 ViewProjection2D;
    uint32_t     DrawMeshlets;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float32_t intensity;
};

struct WorldTransform
{
    float32_t4x4 world;
};

struct Material
{
    float32_t4x4 uvTransform;
    float32_t4 color;
    int32_t enableLighting;
    int32_t textureIndex;
};

struct VertexData
{
    float32_t4 pos;
    float32_t2 texCoord;
    float32_t3 normal;
    float32_t4 color;
    float32_t4 weight;
    int32_t4   jointIndex;
    int32_t    materialIndex;
};

struct Meshlet
{
    uint32_t VertCount;
    uint32_t VertOffset;
    uint32_t PrimCount;
    uint32_t PrimOffset;
};

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

struct VertexOutPut
{
    float32_t4 pos      : SV_POSITION0;
    float32_t2 texCoord : TEXCOORD0;
    float32_t3 normal   : NORMAL0;
    float32_t4 color    : COLOR0;
    int32_t    mIndex   : INDEX0;
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