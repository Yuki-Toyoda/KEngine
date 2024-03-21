struct VertexOutPut
{
    float4 pos   : SV_Position;
    float4 color : COLOR0;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};

cbuffer GeneralData : register(b0)
{
    float4x4 WorldViewProj;
};

float4 TransformPosition(float4 v)
{
    return mul(v, WorldViewProj);
}

StructuredBuffer<Meshlet> Meshlets         : register(t0);
StructuredBuffer<VertexOutPut> Vertices    : register(t1);
StructuredBuffer<uint>    VertexIndices    : register(t2);
StructuredBuffer<uint>    PrimitiveIndices : register(t3);