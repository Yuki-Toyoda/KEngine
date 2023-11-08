// 頂点データ
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
    float4 color;
};

// マテリアル
struct Material
{
    float4x4 uvTransform;
    int enableLighting;
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
    uint vertex; // 実質頂点インデックスの役割
    uint cameraVP; // カメラのビュープロジェクション行列
    uint worldMatrix; // ワールドトランスフォーム
    uint material; // マテリアル
    uint tex2d; // テクスチャ
};


// ** 両方用 ** //

// 各ストラクチャーバッファーのインデックス
StructuredBuffer<IndexInfo> gIndex : register(t0);
// 平行光源
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b0);


// ** VS用 ** //

// 頂点データ
StructuredBuffer<Vertex> gVertex : register(t1);
// カメラのViewProjection
StructuredBuffer<float4x4> gCameraVP : register(t2);
// WorldTransform
StructuredBuffer<float4x4> gWorldMatrix : register(t3);


// ** PS用 ** //

// マテリアル
StructuredBuffer<Material> gMaterial : register(t1);

// シャドウマップのテクスチャ
Texture2D<float> gShadowMap : register(t2);
SamplerState gShadowMapSampler : register(s0);
// テクスチャ
Texture2D<float4> gTexture[] : register(t3);
SamplerState gSampler : register(s1);


// 頂点シェーダーの出力結果の構造体
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 color : COLOR0;
    uint id : SV_InstanceID; // VSで参照したIndexInfoの番号
};