#include "Model.h"
#include "../../../Debug/Debug.h"
#include <fstream>
#include <unordered_set>

using namespace DirectX;
using namespace Microsoft::WRL;

// 構造体宣言
namespace {

    /// <summary>
    /// 入力レイアウト構造体配列
    /// </summary>
    const D3D12_INPUT_ELEMENT_DESC elementDescs[Attribute::Count] = {
        { "POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }, // 座標
        { "NORMAL",   0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }, // 法線
        { "TEXCOORD", 0,  DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }, // テクスチャ座標系
        { "TANGENT", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }, // 接線
        { "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 1 }  // 従法線
    };

    /// <summary>
    /// 各属性データの要素サイズ定義配列
    /// </summary>
    const uint32_t sizeMap_[] = {
        12, // 頂点座標
        12, // 法線
        8,  // テクスチャ座標系
        12, // 接線
        12  // 従法線
    };

    // ファイルの識別子の設定
    const uint32_t c_prolog = 'MSHL';

    // ファイルバージョン列挙子
    enum FileVersion
    {
        FILE_VERSION_INITIAL = 0,                   // 初期バージョン
        CURRENT_FILE_VERSION = FILE_VERSION_INITIAL // 現在のファイルバージョン
    };

    /// <summary>
    /// ファイルヘッダ構造体
    /// </summary>
    struct FileHeader
    {
        uint32_t Prolog;          // ファイル識別子
        uint32_t Version;         // ファイルバージョン

        uint32_t MeshCount;       // メッシュ数
        uint32_t AccessorCount;   // メッシュに関連付けされたアクセッサ数
        uint32_t BufferViewCount; // バッファのビュー数
        uint32_t BufferSize;      // データバッファ数
    };

    /// <summary>
    /// メッシュメタデータ構造体
    /// </summary>
    struct MeshHeader
    {
        uint32_t Indices;                      // インデックスデータまでのアドレス
        uint32_t IndexSubsets;                 // サブセットデータまでのアドレス
        uint32_t Attributes[Attribute::Count]; // 各属性までのアドレス

        uint32_t Meshlets;                     // メッシュレットまでのアドレス
        uint32_t MeshletSubsets;               // メッシュレットのサブセットまでのアドレス
        uint32_t UniqueVertexIndices;          // 固有頂点インデックスデータまでのアドレス
        uint32_t PrimitiveIndices;             // プリミティブインデックスデータまでのアドレス
        uint32_t CullData;                     // カリングデータまでのアドレス
    };

    /// <summary>
    /// バッファビュー情報構造体
    /// </summary>
    struct BufferView
    {
        uint32_t Offset; // バッファ先頭からのオフセット
        uint32_t Size;   // バッファ内のサイズ
    };

    /// <summary>
    /// アクセスのためのアドレス構造体
    /// </summary>
    struct Accessor
    {
        uint32_t BufferView; // アクセスするデータが格納されているバッファのビューまでのアドレス
        uint32_t Offset;     // バッファの先頭からのオフセット
        uint32_t Size;       // アクセスするデータのサイズ
        uint32_t Stride;     // バッファ内の要素間のオフセット
        uint32_t Count;      // アクセスするデータの要素数
    };

    /// <summary>
    /// 指定されたDXGIフォーマットの要素サイズゲッター
    /// </summary>
    /// <param name="format">要素サイズを調べたいフォーマット</param>
    /// <returns>指定されたDXGIフォーマットの要素サイズ</returns>
    uint32_t GetFormatSize(DXGI_FORMAT format)
    {
        // 引数によって処理を変える
        switch (format)
        {
        case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16; // 16バイト
        case DXGI_FORMAT_R32G32B32_FLOAT: return 12;    // 12バイト
        case DXGI_FORMAT_R32G32_FLOAT: return 8;        // 8バイト
        case DXGI_FORMAT_R32_FLOAT: return 4;           // 4バイト
        // 対応できるフォーマット以外のフォーマットが来た場合例外処理を行う
        default: throw std::exception("Unimplemented type");
        }
    }

    /// <summary>
    /// 整数の除算を行い、結果の切り上げを行う関数
    /// </summary>
    /// <typeparam name="T">整数型1</typeparam>
    /// <typeparam name="U">整数型2</typeparam>
    /// <param name="num">除算を行う値</param>
    /// <param name="denom">除数</param>
    /// <returns></returns>
    template <typename T, typename U>
    constexpr T DivRoundUp(T num, U denom)
    {
        return (num + denom - 1) / denom;
    }

    /// <summary>
    /// 引数の値を特定のアライメントに合わせて調整する関数
    /// </summary>
    /// <typeparam name="T">型</typeparam>
    /// <param name="size">調整するサイズ</param>
    /// <returns>調整されたサイズ</returns>
    template <typename T>
    size_t GetAlignedSize(T size)
    {
        const size_t alignment = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
        const size_t alignedSize = (size + alignment - 1) & ~(alignment - 1);
        return alignedSize;
    }
}

bool Model::LoadFromFile(const wchar_t* filename)
{
    // ファイルをバイナリモードにて開く
    std::ifstream stream(filename, std::ios::binary);
    // ファイルを開けなかった場合
    if (!stream.is_open()) {
        // ログに出力
        Debug::Log("Can't open this file.");
        // 読み込めなかったことを示す
        return false;
    }

    // 読み込み用変数の生成
    std::vector<MeshHeader> meshes;      // メッシュ情報格納用
    std::vector<BufferView> bufferViews; // バッファビュー情報格納用
    std::vector<Accessor> accessors;     // アドレス格納用配列

    // ファイルヘッダー格納用
    FileHeader header;
    // ヘッダー情報をファイルから読み込む
    stream.read(reinterpret_cast<char*>(&header), sizeof(header));

    // 読み込もうとしたファイルの識別子が設定されたものかどうかを確認する
    if (header.Prolog != c_prolog) {
        // ログでヘッダーが一致していないことを出力
        Debug::Log("File header is not match.");
        // 読み込み失敗
        return false;
    }

    // 読み込もうとしたシリアルコードのバージョンが一致しているか確認する
    if (header.Version != CURRENT_FILE_VERSION) {
        // ログで読み込んだファイルのバージョンが一致していないことを出力
        Debug::Log("File version is not match.");
        // 読み込み失敗
        return false;
    }

    /// メッシュのメタデータの読み込みを行う
    // メッシュ情報
    meshes.resize(header.MeshCount);                                                        // ヘッダーの情報を元に配列のリサイズ
    stream.read(reinterpret_cast<char*>(meshes.data()), meshes.size() * sizeof(meshes[0])); // メッシュ情報をファイルから読み込む
    // アドレス情報
    accessors.resize(header.AccessorCount);                                                          // ヘッダーの情報を元に配列のリサイズ
    stream.read(reinterpret_cast<char*>(accessors.data()), accessors.size() * sizeof(accessors[0])); // アドレス情報をファイルから読み込む
    // バッファビュー情報
    bufferViews.resize(header.BufferViewCount);                                                            // ヘッダーの情報を元に配列のリサイズ
    stream.read(reinterpret_cast<char*>(bufferViews.data()), bufferViews.size() * sizeof(bufferViews[0])); // バッファビュー情報をファイルから読み込む
    // バッファ数
    buffer_.resize(header.BufferSize);                                       // ヘッダーの情報を元に配列のリサイズ
    stream.read(reinterpret_cast<char*>(buffer_.data()), header.BufferSize); // バッファのサイズをファイルから読み込む

    // ファイル最後のバイト格納用
    char eofByte;
    // ファイルから最後のバイトを読み込む
    stream.read(&eofByte, 1);

    // ファイルの終端に到達していない場合プログラムを停止させる
    assert(stream.eof());

    // 終端まで到達していた場合、ファイルをクローズする
    stream.close();

    // 読み込んだファイル情報を元にメンバ変数の配列をリサイズする
    meshes_.resize(meshes.size());
    // ファイルから読み込んだ情報を元にメンバ変数に代入する
    for (uint32_t i = 0; i < static_cast<uint32_t>(meshes.size()); i++) {
        // 読み込み済み配列から情報を取得
        auto& meshView = meshes[i];
        // 書き込み用アドレスを取得
        auto& mesh = meshes_[i];

        // インデックス情報の読み込み
        {
            // 情報までのアドレスを求める
            Accessor& accessor = accessors[meshView.Indices];

            // アクセッサを用いて情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];                        // バッファビュー情報
            mesh.indexSize_ = accessor.Size;                                                  // インデックスサイズ
            mesh.IndexCount_ = accessor.Count;                                                // インデックス総数
            mesh.indices_ = MakeSegment(buffer_.data() + bufferView.Offset, bufferView.Size); // インデックス情報の配列を生成
        }
        // インデックスのサブセット情報の読み込み
        {
            // 情報までのアドレスを求める
            Accessor& accessor = accessors[meshView.Indices];

            // アクセッサを用いて情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];                                                       // バッファビュー情報
            mesh.IndexSubSets_ = MakeSegment(reinterpret_cast<Subset*>(buffer_.data() + bufferView.Offset), accessor.Count); // サブセット情報の配列を生成
        }

        /// 頂点データとレイアウト情報の読み込み
        // バッファビューの数を保持する配列
        std::vector<uint32_t> vbMap;

        // レイアウト設定を読み込みメッシュ情報から取得
        mesh.layOutDesc_.pInputElementDescs = mesh.layOutElems_;
        mesh.layOutDesc_.NumElements = 0;

        // 各属性にて、頂点属性分ループする
        for (uint32_t j = 0; j < Attribute::Count; j++) {
            // 存在していない属性だった場合
            if (meshView.Attributes[j] == -1) {
                // 次の属性を検証する
                continue;
            }

            // アドレス格納用配列から属性までのアドレスを取得
            Accessor& accessor = accessors[meshView.Attributes[j]];

            // アドレスを元に頂点バッファ配列から情報を取得する
            auto it = std::find(vbMap.begin(), vbMap.end(), accessor.BufferView);
            // 既に追加されている場合
            if (it != vbMap.end()) {
                // 次の属性を検証する
                continue;
            }

            // 新しいバッファビュー情報だった場合配列にその情報を追加する
            vbMap.push_back(accessor.BufferView);
            // バッファビュー情報を控えておく
            BufferView& bufferView = bufferViews[accessor.BufferView];
            
            // 控えた情報を元に配列生成
            DataSegment<uint8_t> verts = MakeSegment(buffer_.data() + bufferView.Offset, bufferView.Size);

            // 各情報をメンバ変数に書き込み
            mesh.vertexStrides_.push_back(accessor.Stride);                               // 情報間隔
            mesh.vertices_.push_back(verts);                                              // 頂点情報
            mesh.vertexCount_ = static_cast<uint32_t>(verts.GetSize()) / accessor.Stride; // 頂点数
        }

        // 各属性について頂点属性分ループする
        for (uint32_t j = 0; j < Attribute::Count; j++)
        {
            // 存在していない属性だった場合
            if (meshView.Attributes[j] == -1) {
                // 次の属性を検証する
                continue;
            }

            // アドレス格納用配列から属性までのアドレスを取得
            Accessor& accessor = accessors[meshView.Attributes[j]];

            // アドレスを元に頂点バッファ配列から情報を取得する
            auto it = std::find(vbMap.begin(), vbMap.end(), accessor.BufferView);

            // 入力レイアウトを取得
            D3D12_INPUT_ELEMENT_DESC desc = elementDescs[j];
            // 各属性がどの頂点バッファに関連付けられているか調べる
            desc.InputSlot = static_cast<uint32_t>(std::distance(vbMap.begin(), it));

            // 使用される入力要素の数を代入
            mesh.layOutElems_[mesh.layOutDesc_.NumElements++] = desc;
        }

        // メッシュレットの読み込み
        {
            // メッシュレット情報までのアドレスを取得
            Accessor& accessor = accessors[meshView.Meshlets];
            // アドレスを用いてメッシュレット情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];

            // 取得した情報をメンバ変数に代入する
            mesh.meshlets_ = MakeSegment(reinterpret_cast<Meshlet*>(buffer_.data() + bufferView.Offset), accessor.Count);
        }

        // メッシュレットのサブセット情報の読み込み
        {
            // メッシュレットのサブセット情報までのアドレスを取得
            Accessor& accessor = accessors[meshView.MeshletSubsets];
            // アドレスを用いてメッシュレットのサブセット情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];

            // 取得した情報をメンバ変数に代入する
            mesh.meshletSubsets_ = MakeSegment(reinterpret_cast<Subset*>(buffer_.data() + bufferView.Offset), accessor.Count);
        }

        // 固有頂点情報の読み込み
        {
            // 固有頂点情報までのアドレスを取得
            Accessor& accessor = accessors[meshView.UniqueVertexIndices];
            // アドレスを用いて固有頂点情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];

            // 取得した情報をメンバ変数に代入する
            mesh.uniqueVertexIndices_ = MakeSegment(buffer_.data() + bufferView.Offset, bufferView.Size);
        }

        // プリミティブのインデックス情報の読み込み
        {
            // プリミティブのインデックス情報までのアドレス取得
            Accessor& accessor = accessors[meshView.PrimitiveIndices];
            // アドレスを用いてプリミティブのインデックス情報を取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];

            // 取得した情報をメンバ変数に代入する
            mesh.primitiveIndices_ = MakeSegment(reinterpret_cast<PackedTriangle*>(buffer_.data() + bufferView.Offset), accessor.Count);
        }

        // カリングデータ読み込み
        {
            // カリングデータまでのアドレス取得
            Accessor& accessor = accessors[meshView.CullData];
            // アドレスを用いてのカリングデータを取得する
            BufferView& bufferView = bufferViews[accessor.BufferView];

            // 取得した情報をメンバ変数に代入する
            mesh.cullingData_ = MakeSegment(reinterpret_cast<CullData*>(buffer_.data() + bufferView.Offset), accessor.Count);
        }
    }

    // 境界球の生成
    for (uint32_t i = 0; i < static_cast<uint32_t>(meshes_.size()); i++)
    {
        // 書き込み用アドレスを取得
        auto& m = meshes_[i];

        // 位置属性のインデックス格納用
        uint32_t vbIndexPos = 0;

        // 入力レイアウトの数分ループを行う
        for (uint32_t j = 1; j < m.layOutDesc_.NumElements; j++)
        {
            // 入力レイアウト設定を取得
            auto& desc = m.layOutElems_[j];
            // 位置属性が含まれている場合
            if (strcmp(desc.SemanticName, "POSITION") == 0)
            {
                // インデックスを取得
                vbIndexPos = j;
                // ループを抜ける
                break;
            }
        }

        // 位置属性のオフセット格納用
        uint32_t positionOffset = 0;

        // 入力レイアウトの数分ループを行う
        for (uint32_t j = 0; j < m.layOutDesc_.NumElements; j++)
        {
            // 入力レイアウト設定を取得
            auto& desc = m.layOutElems_[j];
            // 位置属性が含まれている場合
            if (strcmp(desc.SemanticName, "POSITION") == 0)
            {
                // ループを抜ける
                break;
            }

            // オフセットを発見した場合
            if (desc.InputSlot == vbIndexPos)
            {
                // オフセットを取得、加算する
                positionOffset += GetFormatSize(m.layOutElems_[j].Format);
            }
        }

        // 境界球の生成に必要な値を計算する
        XMFLOAT3* v0 = reinterpret_cast<XMFLOAT3*>(m.vertices_[vbIndexPos].GetFront() + positionOffset); // 中心座標を求める
        uint32_t stride = m.vertexStrides_[vbIndexPos];                                                  // 情報間隔を求める

        // 境界球を求めた値を元に生成する
        BoundingSphere::CreateFromPoints(m.boundingSphere_, m.vertexCount_, v0, stride);

        // 最初の境界球だった場合
        if (i == 0)
        {
            // 生成したものをメンバ変数に代入する
            boundingSphere_ = m.boundingSphere_;
        }
        else
        {
            // 生成した境界球をメンバ変数の境界球に合成する
            BoundingSphere::CreateMerged(boundingSphere_, boundingSphere_, m.boundingSphere_);
        }
    }

    // 読み込み完了
    return true;
}

bool Model::UploadGpuResources(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
    // 全メッシュ分アップロードを行う
    for (uint32_t i = 0; i < meshes_.size(); i++) {
        // 配列内の１つのメッシュを取得
        auto& m = meshes_[i];

        // それぞれのリソースに適切なサイズを求める
        auto indexDesc =       CD3DX12_RESOURCE_DESC::Buffer(m.indices_.GetSize());                                           // インデックス情報分
        auto meshletDesc =     CD3DX12_RESOURCE_DESC::Buffer(m.meshlets_.GetSize() * sizeof(m.meshlets_[0]));                 // メッシュレットの数 * メッシュレット１つ分
        auto cullDataDesc =    CD3DX12_RESOURCE_DESC::Buffer(m.cullingData_.GetSize() * sizeof(m.cullingData_[0]));           // カリングデータ数 * カリングデータ１つ分
        auto vertexIndexDesc = CD3DX12_RESOURCE_DESC::Buffer(DivRoundUp(m.uniqueVertexIndices_.GetSize(),4) * 4);             // 固有頂点数 / 4 * 4
        auto primitiveDesc =   CD3DX12_RESOURCE_DESC::Buffer(m.primitiveIndices_.GetSize() * sizeof(m.primitiveIndices_[0])); // プリミティブインデックス情報 * プリミティブインデックス情報１つ分
        auto meshInfoDesc =    CD3DX12_RESOURCE_DESC::Buffer(sizeof(MeshInfo));                                               // メッシュ情報構造体１つ分

        // デフォルトのヒープ設定を取得
        auto defaultHeap = D3D12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        // リソースの生成を行う
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.indexResource_));                   // インデックスリソース
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.meshletResource_));               // メッシュレットリソース
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &cullDataDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.cullDataResource_));             // カリングデータリソース
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vertexIndexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.uniqueVertexIndexResource_)); // 固有頂点リソース
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &primitiveDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.primitiveIndexResource_));      // プリミティブインデックスリソース
        device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &meshInfoDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.meshInfoResource_));             // メッシュ情報リソース

        // バッファのアドレスをリソースから取得
        m.IBView.BufferLocation = m.indexResource_->GetGPUVirtualAddress();
        // フォーマットの設定を行う
        m.IBView.Format = m.indexSize_ == 4 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
        // バイトサイズの設定
        m.IBView.SizeInBytes = m.IndexCount_ * m.indexSize_;

        // 頂点バッファ配列を頂点数分リサイズ
        m.vertexResources_.resize(m.vertices_.size());
        // 同じようにインデックスバッファビューもリサイズ
        m.VBViews.resize(m.vertices_.size());

        // 頂点数分ループを行う
        for (uint32_t j = 0; j < m.vertices_.size(); j++) {
            // 頂点リソースのサイズを求める
            auto vertexDesc = CD3DX12_RESOURCE_DESC::Buffer(m.vertices_[j].GetSize());
            // 頂点リソースの生成を行う
            device->CreateCommittedResource(&defaultHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m.vertexResources_[j]));

            // バッファのアドレスをリソースから取得する
            m.VBViews[j].BufferLocation = m.vertexResources_[j]->GetGPUVirtualAddress();
            // バイトサイズの設定
            m.VBViews[j].SizeInBytes = static_cast<uint32_t>(m.vertices_[j].GetSize());
            // バイト間隔の設定
            m.VBViews[j].StrideInBytes = m.vertexStrides_[j];
        }

        // アップロード用のリソースを定義する
        std::vector<ComPtr<ID3D12Resource>> vertexUploads;           // 頂点リソース
        ComPtr<ID3D12Resource>              indexUpload;             // インデックスリソース
        ComPtr<ID3D12Resource>              meshletUpload;           // メッシュレットリソース
        ComPtr<ID3D12Resource>              cullDataUpload;          // カリングデータリソース
        ComPtr<ID3D12Resource>              uniqueVertexIndexUpload; // 固有頂点リソース
        ComPtr<ID3D12Resource>              primitiveIndexUpload;    // プリミティブインデックスリソース
        ComPtr<ID3D12Resource>              meshInfoUpload;          // メッシュ情報リソース

        // アップロード用のヒープ設定を取得
        auto uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        // リソースの生成を行う
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &indexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexUpload));                   // インデックス
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshletDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshletUpload));               // メッシュレット
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &cullDataDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&cullDataUpload));             // カリングデータ
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &vertexIndexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uniqueVertexIndexUpload)); // 固有頂点
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &primitiveDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&primitiveIndexUpload));      // プリミティブインデックス
        device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &meshInfoDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&meshInfoUpload));             // メッシュ情報

        // アップロード用の頂点リソース配列をリサイズする
        vertexUploads.resize(m.vertices_.size());
        // 頂点数分ループを行う
        for (uint32_t j = 0; j < m.vertices_.size(); ++j)
        {
            // 頂点リソースのサイズを求める
            auto vertexDesc = CD3DX12_RESOURCE_DESC::Buffer(m.vertices_[j].GetSize());
            // 頂点リソースの生成を行う
            device->CreateCommittedResource(&uploadHeap, D3D12_HEAP_FLAG_NONE, &vertexDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexUploads[j]));

            /// 頂点データのアップロードを行う
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            vertexUploads[j]->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.vertices_[j].GetFront(), m.vertices_[j].GetSize());
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            vertexUploads[j]->Unmap(0, nullptr);
        }

        {
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            indexUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.indices_.GetFront(), m.indices_.GetSize());
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            indexUpload->Unmap(0, nullptr);
        }

        {
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            meshletUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.meshlets_.GetFront(), m.meshlets_.GetSize() * sizeof(m.meshlets_[0]));
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            meshletUpload->Unmap(0, nullptr);
        }

        {
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            cullDataUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.cullingData_.GetFront(), m.cullingData_.GetSize() * sizeof(m.cullingData_[0]));
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            cullDataUpload->Unmap(0, nullptr);
        }

        {
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            uniqueVertexIndexUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.uniqueVertexIndices_.GetFront(), m.uniqueVertexIndices_.GetSize());
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            uniqueVertexIndexUpload->Unmap(0, nullptr);
        }

        {
            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            primitiveIndexUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, m.primitiveIndices_.GetFront(), m.primitiveIndices_.GetSize() * sizeof(m.primitiveIndices_[0]));
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            primitiveIndexUpload->Unmap(0, nullptr);
        }

        {
            // メッシュ情報格納用
            MeshInfo info = {};
            // 各種情報の取得
            info.indexSize_ = m.indexSize_;                                   // インデックスサイズ
            info.meshletSize_ = static_cast<uint32_t>(m.meshlets_.GetSize()); // メッシュレットの情報
            info.lastMeshletVertCount_ = m.meshlets_.GetBack().vertCount_;    // 最後のメッシュレットの頂点数
            info.lastMeshletPrimCount_ = m.meshlets_.GetBack().primCount_;    // 最後のメッシュレットのプリミティブ数

            // GPUリソースのメモリに対するポインタ格納用
            uint8_t* memory = nullptr;
            // マッピングを行う
            meshInfoUpload->Map(0, nullptr, reinterpret_cast<void**>(&memory));
            // 読み込まれたデータのサイズを取得する
            std::memcpy(memory, &info, sizeof(MeshInfo));
            // データのコピーが完了し次第、GPUのリソースのアンマップを行う
            meshInfoUpload->Unmap(0, nullptr);
        }

        // 頂点数分ループを行う
        for (uint32_t j = 0; j < m.vertices_.size(); ++j)
        {
            // GPUリソースにCPUリソースをコピーする
            cmdList->CopyResource(m.vertexResources_[j].Get(), vertexUploads[j].Get());
            // リソースの状態を変更する
            const auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m.vertexResources_[j].Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
            // 操作の完了をGPU側に通知する
            cmdList->ResourceBarrier(1, &barrier);
        }

        // 処理後のリソース格納用
        D3D12_RESOURCE_BARRIER postCopyBarriers[6];

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.indexResource_.Get(), indexUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[0] = CD3DX12_RESOURCE_BARRIER::Transition(m.indexResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.meshletResource_.Get(), meshletUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[1] = CD3DX12_RESOURCE_BARRIER::Transition(m.meshletResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.cullDataResource_.Get(), cullDataUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[2] = CD3DX12_RESOURCE_BARRIER::Transition(m.cullDataResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.uniqueVertexIndexResource_.Get(), uniqueVertexIndexUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[3] = CD3DX12_RESOURCE_BARRIER::Transition(m.uniqueVertexIndexResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.primitiveIndexResource_.Get(), primitiveIndexUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[4] = CD3DX12_RESOURCE_BARRIER::Transition(m.primitiveIndexResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

        // GPUリソースにCPUリソースをコピーする
        cmdList->CopyResource(m.meshInfoResource_.Get(), meshInfoUpload.Get());
        // リソースの状態を変更する
        postCopyBarriers[5] = CD3DX12_RESOURCE_BARRIER::Transition(m.meshInfoResource_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);

        // 操作の完了をGPU側に通知する
        cmdList->ResourceBarrier(ARRAYSIZE(postCopyBarriers), postCopyBarriers);
    }

    return true;
}
