#include "Meshlet.hlsli"

/////
// Data Loaders

//uint3 UnpackPrimitive(uint primitive)
//{
//    // Unpacks a 10 bits per index triangle from a 32-bit uint.
//    return uint3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
//}

//uint3 GetPrimitive(Meshlet m, uint index)
//{
//    return UnpackPrimitive(PrimitiveIndices[m.PrimOffset + index]);
//}

//uint GetVertexIndex(Meshlet m, uint localIndex)
//{
//    localIndex = m.VertOffset + localIndex;

//   // Byte address must be 4-byte aligned.
//    uint wordOffset = (localIndex & 0x1);
//    uint byteOffset = (localIndex / 2) * 4;

//    // Grab the pair of 16-bit indices, shift & mask off proper 16-bits.
//    uint indexPair = VertexIndices.Load(byteOffset);
//    uint index = (indexPair >> (wordOffset * 16)) & 0xffff;

//    return index;
//}

VertexOutPut GetVertexAttribute(uint vertexIndex, uint meshletIndex)
{
    Vertex v = Vertices[vertexIndex];

    VertexOutPut vout;
    vout.MeshletIndex = meshletIndex;

    return vout;
}

[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    in uint gid : SV_GroupID,
    in uint gtid : SV_GroupThreadID,
    out vertices VertexOutPut verts[128],
    out indices uint3         tris[128]
)
{
    Meshlet meshlet = Meshlets[gid]; // Meshlet取得

    SetMeshOutputCounts(meshlet.VertCount, meshlet.PrimCount);

    uint vertexIndex = VertexIndices[meshlet.VertOffset + gtid];
    
    if (gtid < meshlet.VertCount)
    {   
        Vertex vertex = Vertices[vertexIndex];
        
        verts[gtid].pos = TransformPosition(vertex.Position);
    }
    if (gtid < meshlet.PrimCount)
    {
        uint packedIndices = PrimitiveIndices[meshlet.PrimOffset + gtid];
        
        tris[gtid] = uint3(packedIndices & 0xFF,
                          (packedIndices >> 8) & 0xFF,
                          (packedIndices >> 16) & 0xFF
                        );
        
        verts[gtid] = GetVertexAttribute(vertexIndex, gid); // 頂点を出力
        
    }

}