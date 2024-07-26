#include "../Constant.hlsli"

struct Well
{
    float32_t4x4 skeltonSpaceMatrix;
    float32_t4x4 skeltonSpaceInverseTransposeMatrix;
};

ConstantBuffer<int32_t>        VertCount      : register(b0);
StructuredBuffer<Well>         gMatrixPalette : register(t0);
StructuredBuffer<VertexData>   InputVertices  : register(t1);
RWStructuredBuffer<VertexData> OutputVertices : register(u0);

[numthreads(1024, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    // 現在計算を行おうとしている頂点インデックスを取得
    uint32_t vertexIndex = DTid.x;
    
    // 頂点数を超過していない場合以下の処理を行う
    if (vertexIndex < VertCount)
    {
        // 計算する頂点を取得
        VertexData v = InputVertices[vertexIndex];
        
        // スキニング後の頂点格納用
        VertexData skinned;
        // テクスチャ座標系はそのまま取得
        skinned.texCoord = v.texCoord;
        
        // スキニングの計算を行う
        skinned.pos  = mul(v.pos, gMatrixPalette[v.jointIndex.x].skeltonSpaceMatrix) * v.weight.x;
        skinned.pos += mul(v.pos, gMatrixPalette[v.jointIndex.y].skeltonSpaceMatrix) * v.weight.y;
        skinned.pos += mul(v.pos, gMatrixPalette[v.jointIndex.z].skeltonSpaceMatrix) * v.weight.z;
        skinned.pos += mul(v.pos, gMatrixPalette[v.jointIndex.w].skeltonSpaceMatrix) * v.weight.w;
        skinned.pos.w = 1.0f;
    
        skinned.normal  = mul(v.normal, (float32_t3x3) gMatrixPalette[v.jointIndex.x].skeltonSpaceInverseTransposeMatrix) * v.weight.x;
        skinned.normal += mul(v.normal, (float32_t3x3) gMatrixPalette[v.jointIndex.y].skeltonSpaceInverseTransposeMatrix) * v.weight.y;
        skinned.normal += mul(v.normal, (float32_t3x3) gMatrixPalette[v.jointIndex.z].skeltonSpaceInverseTransposeMatrix) * v.weight.z;
        skinned.normal += mul(v.normal, (float32_t3x3) gMatrixPalette[v.jointIndex.w].skeltonSpaceInverseTransposeMatrix) * v.weight.w;
        skinned.normal = normalize(skinned.normal);
        
        // 計算後の頂点を格納する
        OutputVertices[vertexIndex] = skinned;
    }
}