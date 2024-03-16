#define ROOT_SIG_MS "CBV(b0), \
                    SRV(t0), \
                    SRV(t1), \
                    SRV(t2), \
                    SRV(t3)"

struct Constants
{
    float4x4 World;
    float4x4 WorldView;
    float4x4 WorldViewProj;
    uint DrawMeshlets;
};

struct Vertex
{
    float3 Position;
    float3 Normal;
};

struct VertexOutPut
{
    float4 PositionHS : SV_Position;
    float3 PositionVS : POSITION0;
    float3 Normal : NORMAL0;
    uint MeshletIndex : COLOR0;
};

struct Meshlet
{
    uint VertCount;
    uint VertOffset;
    uint PrimCount;
    uint PrimOffset;
};