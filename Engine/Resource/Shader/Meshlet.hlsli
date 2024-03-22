struct VertexData
{
    float4 pos;
    float4 color;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};

struct VertexOutPut
{
    float4 pos : SV_Position;
    float4 color : COLOR0;
};

cbuffer GeneralData : register(b0)
{
    matrix WorldViewProj;
};

float4 TransformPosition(float4 v)
{
    return mul(v, WorldViewProj);
}

StructuredBuffer<Meshlet>    Meshlets         : register(t0);
StructuredBuffer<VertexData> Vertices         : register(t1);
StructuredBuffer<uint>       VertexIndices    : register(t2);
StructuredBuffer<uint>       PrimitiveIndices : register(t3);