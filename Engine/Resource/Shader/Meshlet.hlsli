//#define ROOT_SIG_MS "CBV(b0), \
//                     SRV(t0), \
//                     SRV(t1), \
//                     SRV(t2), \
//                     SRV(t3)"

struct GeneralData
{
    float4x4 WorldViewProj;
    uint DrawMeshlets;
};

struct Vertex
{
    float4 Position;
};

struct VertexOutPut
{
    float4 pos        : SV_Position;
    uint MeshletIndex : COLOR0;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};

ConstantBuffer<GeneralData> Globals : register(b0);

float4 TransformPosition(float4 v)
{
    return mul(v, Globals.WorldViewProj);
}

StructuredBuffer<Vertex>  Vertices         : register(t0);
StructuredBuffer<Meshlet> Meshlets         : register(t1);
StructuredBuffer<uint>    VertexIndices    : register(t2);
StructuredBuffer<uint>    PrimitiveIndices : register(t3);