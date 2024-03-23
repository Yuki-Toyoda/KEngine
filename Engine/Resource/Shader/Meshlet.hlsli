struct ConstantData
{
    float4x4 WorldViewProj;
};

struct VertexData
{
    float32_t4 pos;
    float32_t4 color;
};

struct Meshlet
{
    uint32_t VertCount;
    uint32_t VertOffset;
    uint32_t PrimCount;
    uint32_t PrimOffset;
};

struct VertexOutPut
{
    float4 pos : SV_POSITION0;
    float4 color : COLOR0;
};

ConstantBuffer<ConstantData> ConstantData : register(b0);

float4 TransformPosition(float4 v)
{
    return mul(v, ConstantData.WorldViewProj);
}

StructuredBuffer<Meshlet>    Meshlets         : register(t0);
StructuredBuffer<VertexData> Vertices         : register(t1);
StructuredBuffer<uint32_t>   VertexIndices    : register(t2);
StructuredBuffer<uint32_t3>  PrimitiveIndices : register(t3);