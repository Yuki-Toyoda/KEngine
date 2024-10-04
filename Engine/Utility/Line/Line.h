#pragma once
#include <string>
#include "../../GameObject/IObject.h"
#include "../../Primitive/PrimitiveManager.h"
#include "../../Resource/Texture/TextureManager.h"

/// <summary>
/// 線描画用クラス
/// </summary>
class Line
{
private: // サブクラス

	/// <summary>
	/// 軌跡用バッファ
	/// </summary>
	struct TrailBuffer {
		Vector3 start{};		// 始点
		Vector3 end{};			// 終点
		bool	isUsed = false;	// 使用済みフラグ
	};

	/// <summary>
	/// 軌跡用頂点座標
	/// </summary>
	struct TrailVertex {
		Vector4 pos;
		Vector2 uv;
	};

public: // コンストラクタ等

	// コンストラクタ
	Line() = default;
	// デストラクタ
	~Line();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="position">線の開始座標</param>
	/// <param name="thickness">線の太さ</param>
	/// <param name="length">線の長さ</param>
	/// <param name="texture">(任意)軌跡テクスチャ</param>
	void Init(const std::string& name, const Vector3& position, const Vector2& thickness, const float& length, Texture texture = TextureManager::GetInstance()->GetDefaultTexture());

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList6* cmdList);

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 軌跡更新関数
	/// </summary>
	void TrailUpdate();

	/// <summary>
	/// <para>コライダー追加関数</para>
	/// 第二引数のオブジェクトに関しては、OnCollision関係のコールバック関数の対象になります
	/// </summary>
	/// <param name="name">コライダー名</param>
	/// <param name="object">コライダーがコールバックを行う対象のオブジェクト</param>
	void AddCollider(const std::string& name, IObject* object);

public: // アクセッサ等

	/// <summary>
	/// ワールドトランスフォームゲッター
	/// </summary>
	/// <returns>ワールドトランスフォーム</returns>
	WorldTransform* GetWorldTransform() { return &transform_; }

	/// <summary>
	/// 親子関係セッター。(使い方はワールドトランスフォームと一緒)
	/// </summary>
	/// <param name="parent">セットするワールド座標</param>
	/// <param name="parentType">親子関係フラグ</param>
	void SetParent(WorldTransform* parent, uint8_t parentType = 0b111);

	/// <summary>
	/// 使用済みの軌跡バッファを抽出した配列を取得するゲッター
	/// </summary>
	std::vector<TrailBuffer> GetUsedTrailBuffer();

private: // メンバ変数

	// (いじらないでね)ワールド座標
	WorldTransform transform_;
	// 線の色
	Vector4 color_;

public: // パブリックなメンバ変数

	// 線1つの名前
	std::string name_;
	// 線の始点座標
	Vector3 position_;
	// 線の太さ
	Vector2 thickness_;
	// 始端座標からの長さ
	float length_;
	// 回転
	Vector3 rotate_;

	// 親がいる場合のワールド座標
	Vector3 worldPos_;

	// 表示状態
	bool isActive_ = true;

	// 軌跡表示トリガー
	bool isDisplayTrail_ = false;
	// 軌跡座標配列の要素数
	int trailBufferCount_ = 4;
	// 軌跡用座標配列
	std::vector<TrailBuffer> trailBuffers_;
	// 軌跡の現在座標
	TrailBuffer	tempTrail_{};
	// 軌跡用マテリアル
	Material trailMaterial_{};

	// 軌跡用頂点配列
	std::vector<TrailVertex> vertices_;
	// 軌跡用インデックス配列
	std::vector<uint32_t> indices_;
	// 軌跡用頂点バッファ
	std::unique_ptr<StructuredBuffer<TrailVertex>> vertexBuffer_;
	// 軌跡用インデックスバッファ
	std::unique_ptr<StructuredBuffer<uint32_t>>	indexBuffer_;
};

