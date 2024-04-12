#include "Meshlet.hlsli"

uint32_t3 UnpackPrimitive(uint primitive)
{
    // Unpacks a 10 bits per index triangle from a 32-bit uint.
    return uint32_t3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
}

uint32_t3 GetPrimitive(Meshlet m, uint index)
{
    return UnpackPrimitive(PrimitiveIndices[m.PrimOffset + index]);
}

uint32_t GetVertexIndex(Meshlet m, uint localIndex)
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

[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    in uint32_t gid  : SV_GroupID,
    in uint32_t gtid : SV_GroupThreadID,
    out vertices VertexOutPut outVerts[128],
    out indices uint32_t3     outIndices[128]
)
{
    // Meshlet取得
    Meshlet meshlet = Meshlets[gid];

    // メッシュレット出力数を求める
    SetMeshOutputCounts(meshlet.VertCount, meshlet.PrimCount);

    if (gtid < meshlet.VertCount)
    {   
        // 頂点インデックスの取得
        uint32_t vertexIndex = GetVertexIndex(meshlet, gtid);
        
        // 取得したインデックスから頂点座標を求める
        VertexData vertex = Vertices[vertexIndex];
        
        // 出力する頂点座標を求める
        outVerts[gtid].pos = TransformPosition(vertex.pos);
        
        // 出力する頂点色を求める
        outVerts[gtid].color = float4(
            float(gtid & 1),
            float(gtid & 3) / 4,
            float(gtid & 7) / 8,
            1.0f);
    }
    if (gtid < meshlet.PrimCount)
    {
        // プリミティブ情報のインデックス情報を取得
        uint32_t3 packedIndices = GetPrimitive(meshlet, gtid);
        
        // 出力するプリミティブを求める
        outIndices[gtid] = packedIndices;
    }

}