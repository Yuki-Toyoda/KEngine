#include "Meshlet.hlsli"

ConstantBuffer<Constants> Globals : register(b0);

StructuredBuffer<Vertex> Vertices : register(t0);
StructuredBuffer<Meshlet> Meshlets : register(t1);
ByteAddressBuffer UniqueVertexIndices : register(t2);
StructuredBuffer<uint> PrimitiveIndices : register(t3);

/////
// Data Loaders

uint3 UnpackPrimitive(uint primitive)
{
    // Unpacks a 10 bits per index triangle from a 32-bit uint.
    return uint3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
}

uint3 GetPrimitive(Meshlet m, uint index)
{
    return UnpackPrimitive(PrimitiveIndices[m.PrimOffset + index]);
}

uint GetVertexIndex(Meshlet m, uint localIndex)
{
    localIndex = m.VertOffset + localIndex;

   // Byte address must be 4-byte aligned.
    uint wordOffset = (localIndex & 0x1);
    uint byteOffset = (localIndex / 2) * 4;

    // Grab the pair of 16-bit indices, shift & mask off proper 16-bits.
    uint indexPair = UniqueVertexIndices.Load(byteOffset);
    uint index = (indexPair >> (wordOffset * 16)) & 0xffff;

    return index;
}

VertexOutPut GetVertexAttribute(uint vertexIndex, uint meshletIndex)
{
    Vertex v = Vertices[vertexIndex];

    VertexOutPut vout;
    vout.PositionVS = mul(float4(v.Position, 1), Globals.WorldView).xyz;
    vout.PositionHS = mul(float4(v.Position, 1), Globals.WorldViewProj);
    vout.Normal = mul(float4(v.Normal, 0), Globals.World).xyz;
    vout.MeshletIndex = meshletIndex;

    return vout;
}

[RootSignature(ROOT_SIG_MS)]
[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    uint gtid : SV_GroupThreadID,
    uint gid : SV_GroupID,
    out indices uint3 tris[256],
    out vertices VertexOutPut verts[256]
)
{
    Meshlet m = Meshlets[gid]; // Meshlet取得

    SetMeshOutputCounts(m.VertCount, m.PrimCount);

    if (gtid < m.PrimCount)
    {
        tris[gtid] = GetPrimitive(m, gtid); // 頂点インデックスを設定
    }

    if (gtid < m.VertCount)
    {
        uint vertexIndex = GetVertexIndex(m, gtid);
        verts[gtid] = GetVertexAttribute(vertexIndex, gid); // 頂点を出力
    }
}