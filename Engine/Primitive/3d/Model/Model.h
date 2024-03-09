#pragma once
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"
#include "../../../Math/Matrix4x4.h"
#include "../../../Utility/DataSegment/DataSegment.h"
#include <d3dx12.h>
#include <DirectXCollision.h>

/// 構造体定義

/// <summary>
/// 頂点属性構造体
/// </summary>
struct Attribute {
	// 頂点属性種類列挙子
	enum VAType {
        Position,  // 頂点座標
        Normal,    // 法線
        TexCoord,  // テクスチャ座標系
        Tangent,   // 接線
        Bitangent, // 従法線
        Count      // 属性種類数
	};

    VAType   type_;   // 頂点属性種類
    uint32_t offset_; // 属性のメモリ上の開始位置
};

/// <summary>
/// サブセット構造体
/// </summary>
struct Subset {
    uint32_t offset_; // サブセット始点
    uint32_t count_;  // 頂点数、インデックス数など
};

/// <summary>
/// メッシュ情報構造体
/// </summary>
struct MeshInfo {
    uint32_t indexSize_;            // インデックス数
    uint32_t meshletSize_;          // メッシュレットの数

    uint32_t lastMeshletVertCount_; // 最後のメッシュレットの頂点数
    uint32_t lastMeshletPrimCount_; // 最後のメッシュレットのプリミティブ数
};

/// <summary>
/// メッシュレット構造体
/// </summary>
struct Meshlet {
    uint32_t vertCount_;  // 頂点数
    uint32_t vertOffset_; // 頂点バッファ内の開始位置
    uint32_t primCount_;  // プリミティブ数
    uint32_t primOffset_; // インデックスバッファ内の開始位置
};

/// <summary>
/// <para>頂点インデックス保持用構造体</para>
/// <para>最低限のバイト数で値を保持することでメモリ節約を図る</para>
/// </summary>
struct PackedTriangle {
    uint32_t i0 : 10; // 最初の頂点
    uint32_t i1 : 10; // 2点目
    uint32_t i2 : 10; // 3点目
};

/// <summary>
/// カリング情報構造体
/// </summary>
struct CullData {
    Vector4 BoundingSphere_; // x,y,zまでは中心座標、wは球半径
    uint8_t NormalCone_[4];  // 初め3要素は法線軸、最終要素は法線錘
    float   apexOffset_;     // メッシュの外接球と視錘台頂点までの距離
};

/// <summary>
/// メッシュ構造体
/// </summary>
struct Mesh {
    // 入力要素格納用
    D3D12_INPUT_ELEMENT_DESC LayOutElems[Attribute::Count];
    // 入力レイアウト設定用
    D3D12_INPUT_LAYOUT_DESC LayOutDesc;

    std::vector<DataSegment<uint8_t>> Vertices;       // 頂点情報配列
    std::vector<uint32_t>             VertexStrides;  // 頂点情報間隔配列
    uint32_t                          VertexCount;    // 頂点数
    DirectX::BoundingSphere           BoundingSphere; // (カリング用)境界球

    DataSegment<Subset>  IndexSubSets; // インデックスサブセット情報配列
    DataSegment<uint8_t> Indices;      // インデックス情報配列
    uint32_t             IndexSize;    // インデックスサイズ
    uint32_t             IndexCount;   // インデックス総数

    DataSegment<Subset>         MeshletSubsets;      // メッシュレットサブセット情報配列
    DataSegment<Meshlet>        Meshlets;            // メッシュレット情報配列
    DataSegment<uint8_t>        UniqueVertexIndices; // 固有頂点インデックス情報配列
    DataSegment<PackedTriangle> PrimitiveIndices;    // プリミティブインデックス情報配列
    DataSegment<CullData>       CullingData;         // プリミティブインデックス情報配列

    std::vector<D3D12_VERTEX_BUFFER_VIEW> VBViews; // 頂点バッファビュー情報配列
    D3D12_INDEX_BUFFER_VIEW               IBView;  // インデックスバッファビュー情報

    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> VertexResources;           // 頂点バッファリソース 
    Microsoft::WRL::ComPtr<ID3D12Resource>              IndexResource;             // インデックスバッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              MeshletResource;           // メッシュレットバッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              UniqueVertexIndexResource; // 固有頂点バッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              PrimitiveIndexResource;    // プリミティブバッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              CullDataResource;          // カリング用バッファリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              MeshInfoResource;          // メッシュ情報バッファリソース

    // アップロード用リソース
    std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> VertexUploads;           // 頂点リソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              IndexUpload;             // インデックスリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              MeshletUpload;           // メッシュレットリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              CullDataUpload;          // カリングデータリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              UniqueVertexIndexUpload; // 固有頂点リソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              PrimitiveIndexUpload;    // プリミティブインデックスリソース
    Microsoft::WRL::ComPtr<ID3D12Resource>              MeshInfoUpload;          // メッシュ情報リソース

    /// <summary>
    /// 引数で指定したメッシュレットのパック化数のゲッター
    /// </summary>
    /// <param name="subsetIndex">取得するサブセット</param>
    /// <param name="maxGroupVerts">パック化する頂点の最大数</param>
    /// <param name="maxGroupPrims">パック化するプリミティブの最大数</param>
    /// <returns></returns>
    uint32_t GetLastMeshletPackCount(uint32_t subsetIndex, uint32_t maxGroupVerts, uint32_t maxGroupPrims)
    {
        // メッシュレットの数がそもそも0だった場合0を変えす
        if (Meshlets.GetSize() == 0)
            return 0;

        // 指定されたサブセットとメッシュレットを取得
        auto& subset = MeshletSubsets[subsetIndex];                   // サブセット
        auto& meshlet = Meshlets[subset.offset_ + subset.count_ - 1]; // メッシュレット

        // 取得したメッシュレットの頂点数とプリミティブ数を割り、その最小値を求める
        return min(maxGroupVerts / meshlet.vertCount_, maxGroupPrims / meshlet.primCount_);
    }

    /// <summary>
    /// 引数で指定したプリミティブの頂点ゲッター
    /// </summary>
    /// <param name="index">取得するインデックス</param>
    /// <param name="i0">取得した値を代入する変数0</param>
    /// <param name="i1">取得した値を代入する変数1</param>
    /// <param name="i2">取得した値を代入する変数2</param>
    void GetPrimitive(uint32_t index, uint32_t& i0, uint32_t& i1, uint32_t& i2) const
    {
        // 指定されたインデックスの値を取得
        auto primitive = PrimitiveIndices[index];

        // 取得した値を各変数に代入する
        i0 = primitive.i0;
        i1 = primitive.i1;
        i2 = primitive.i2;
    }

    /// <summary>
    /// 指定された頂点インデックス情報のゲッター
    /// </summary>
    /// <param name="index">取得するインデックス</param>
    /// <returns>引数で指定した頂点インデックス情報</returns>
    uint32_t GetVertexIndex(uint32_t index) const
    {
        // 頂点インデックス情報が格納されているアドレスを求める
        const uint8_t* address = UniqueVertexIndices.GetFront() + index * IndexSize;
       
        // インデックスサイズが4バイトの場合
        if (IndexSize == 4)
        {
            // uint32_t型にキャストして返す
            return *reinterpret_cast<const uint32_t*>(address);
        }
        else
        {
            // uint16_t型にキャストして返す
            return *reinterpret_cast<const uint16_t*>(address);
        }
    }
};

/// <summary>
/// モデルクラス
/// </summary>
class Model
{
public: // メンバ関数

    /// <summary>
    /// ファイルからOBJを読み込む関数
    /// </summary>
    /// <param name="filename">ディレクトリパスを含むファイル名</param>
    /// <returns>ロード成否</returns>
    bool LoadFromFile(const wchar_t* filename);

    /// <summary>
    /// 読み込んだファイルをGPUリソースにアップロードする関数
    /// </summary>
    /// <param name="device">デバイス</param>
    /// <param name="cmdList">コマンドリスト</param>
    /// <returns>アップロード成否</returns>
    bool UploadGpuResources(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
    
public: // アクセッサ等

    /// <summary>
    /// メッシュ数ゲッター
    /// </summary>
    /// <returns>読み込みモデルのメッシュ数</returns>
    uint32_t GetMeshCount() const { return static_cast<uint32_t>(meshes_.size()); }

    /// <summary>
    /// 指定したメッシュ本体のゲッター
    /// </summary>
    /// <param name="i">取得するインデックス</param>
    /// <returns>指定したメッシュ本体</returns>
    const Mesh& GetMesh(uint32_t i) const { return meshes_[i]; }

    /// <summary>
    /// メッシュの境界球のゲッター
    /// </summary>
    /// <returns>メッシュの境界球</returns>
    const DirectX::BoundingSphere& GetBoundingSphere() const { return boundingSphere_; }

    /// <summary>
    /// (イテレータ用)開始関数
    /// </summary>
    /// <returns>配列の始端値</returns>
    auto begin() { return meshes_.begin(); }
    /// <summary>
    /// (イテレータ用)終了関数
    /// </summary>
    /// <returns>配列の終端値</returns>
    auto end() { return meshes_.end(); }

private:

    // メッシュ情報配列
    std::vector<Mesh>                      meshes_;
    // 境界球
    DirectX::BoundingSphere                boundingSphere_;

    // 一時バッファ
    std::vector<uint8_t>                   buffer_;
};

