// 頂点
struct Vertex
{
    float4 position;// 位置座標
    float2 texcoord;// テクスチャ座標
    float3 normal;// 法線
    float4 color;// 色(頂点ごと)
};

// マテリアル
struct Material
{
    float4x4 uvTransform;// uvトランスフォーム
    int enableLighting;// ライティング有効トリガー
};

// 平行光源
struct DirectionalLight
{
    float4x4 viewProjection;
    float4 color; // !< ライトの色
    float3 direction; // !< ライトの向き
    float intensity; // !< 輝度
};

// インデックス情報の構造体
struct IndexInfo
{
    uint vertex;// 頂点
    uint cameraVP;// ビュープロジェクション行列
    uint worldMatrix;// ワールドトランスフォーム
    uint material;// マテリアル
    uint tex2d;// テクスチャ
};


// インデックス情報
StructuredBuffer<IndexInfo> gIndex : register(t0);
// 平行光源
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b0);

// 頂点データ
StructuredBuffer<Vertex> gVertex : register(t1);
// カメラのViewProjection
StructuredBuffer<float4x4> gCameraVP : register(t2);
// WorldTransform
StructuredBuffer<float4x4> gWorldMatrix : register(t3);

// マテリアル
StructuredBuffer<Material> gMaterial : register(t1);

// テクスチャ
Texture2D<float4> gTexture[] : register(t2);
SamplerState gSampler : register(s0);


// 頂点シェーダーの出力結果の構造体
struct VertexShaderOutput
{
    float4 position : SV_Position0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    uint id : SV_InstanceID; // VSで参照したIndexInfoの番号
};