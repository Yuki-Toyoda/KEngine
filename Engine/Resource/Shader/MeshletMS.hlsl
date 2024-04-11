#include "Meshlet.hlsli"

[NumThreads(128, 1, 1)]
[OutputTopology("triangle")]
void main(
    in uint32_t gid  : SV_GroupID,
    in uint32_t gtid : SV_GroupThreadID,
    out vertices VertexOutPut outVerts[128],
    out indices uint32_t3         outIndices[128]
)
{
    // Meshlet取得
    Meshlet meshlet = Meshlets[gid];

    // メッシュレット出力数を求める
    SetMeshOutputCounts(meshlet.VertCount, meshlet.PrimCount);

    if (gtid < meshlet.VertCount)
    {   
        // 頂点インデックスの取得
        uint32_t vertexIndex = VertexIndices[meshlet.VertOffset + gtid];
        
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
        uint32_t3 packedIndices = uint32_t3(PrimitiveIndices[meshlet.PrimOffset + gtid].i0,
                                            PrimitiveIndices[meshlet.PrimOffset + gtid].i1,
                                            PrimitiveIndices[meshlet.PrimOffset + gtid].i2);
        
        // 出力するプリミティブを求める
        outIndices[gtid] = packedIndices;
    }

}