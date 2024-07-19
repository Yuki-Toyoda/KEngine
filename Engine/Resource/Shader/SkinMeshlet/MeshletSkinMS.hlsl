#include "MeshletSkin.hlsli"

uint32_t3 UnpackPrimitive(uint primitive)
{
    // 10ビットごとにプリミティブのインデックスを読み取る
    return uint32_t3(primitive & 0x3FF, (primitive >> 10) & 0x3FF, (primitive >> 20) & 0x3FF);
}

uint32_t3 GetPrimitive(Meshlet m, uint index)
{
    // 面情報取得
    return UnpackPrimitive(PrimitiveIndices[m.PrimOffset + index]);
}

uint32_t GetVertexIndex(Meshlet m, uint localIndex)
{
    // 頂点のインデックスを求める
    localIndex = m.VertOffset + localIndex;

    // 4バイトずつで読み込む
    return UniqueVertexIndices.Load(localIndex * 4);
}

Skinned Skinning(VertexData v)
{
    Skinned skinned;
    skinned.position =  mul(v.pos, gMatrixPalette[v.jointIndex.x].skeltonSpaceMatrix) * v.weight.x;
    skinned.position += mul(v.pos, gMatrixPalette[v.jointIndex.y].skeltonSpaceMatrix) * v.weight.y;
    skinned.position += mul(v.pos, gMatrixPalette[v.jointIndex.z].skeltonSpaceMatrix) * v.weight.z;
    skinned.position += mul(v.pos, gMatrixPalette[v.jointIndex.w].skeltonSpaceMatrix) * v.weight.w;
    skinned.position.w = 1.0f;
    
    skinned.normal =  mul(v.normal, (float32_t3x3)gMatrixPalette[v.jointIndex.x].skeltonSpaceInverseTransposeMatrix) * v.weight.x;
    skinned.normal += mul(v.normal, (float32_t3x3)gMatrixPalette[v.jointIndex.y].skeltonSpaceInverseTransposeMatrix) * v.weight.y;
    skinned.normal += mul(v.normal, (float32_t3x3)gMatrixPalette[v.jointIndex.z].skeltonSpaceInverseTransposeMatrix) * v.weight.z;
    skinned.normal += mul(v.normal, (float32_t3x3)gMatrixPalette[v.jointIndex.w].skeltonSpaceInverseTransposeMatrix) * v.weight.w;
    skinned.normal = normalize(skinned.normal);
    
    return skinned;
}

[NumThreads(128, 1, 1)]      // スレッド数最大128
[OutputTopology("triangle")] // 出力形状は三角形
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
        
        // スキニングを行う
        Skinned skinned = Skinning(vertex);
        
        // 出力する頂点座標を求める
        outVerts[gtid].pos      = mul(skinned.position, mul(Transform.world, ConstantData.WorldViewProj));
        outVerts[gtid].texCoord = vertex.texCoord;
        outVerts[gtid].normal = normalize(mul(skinned.normal, (float32_t3x3)Transform.world));
        outVerts[gtid].mIndex = vertex.materialIndex;
        
        // 出力する頂点色を求める
        if (ConstantData.DrawMeshlets == true)
        {
           // 出力する頂点色を求める
            outVerts[gtid].color = float4(
            float(gid & 1),
            float(gid & 3) / 4,
            float(gid & 7) / 8,
            1.0f);
        }
        else
        {
            // 頂点色はマテリアルから取得
            outVerts[gtid].color = materials[vertex.materialIndex].color;
        }
    }
    if (gtid < meshlet.PrimCount)
    {
        // プリミティブ情報のインデックス情報を取得
        uint32_t3 packedIndices = GetPrimitive(meshlet, gtid);
        
        // 出力するプリミティブを求める
        outIndices[gtid] = packedIndices;
    }

}