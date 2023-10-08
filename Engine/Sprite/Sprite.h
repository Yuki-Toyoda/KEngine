#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include "../Base/DirectXCommon.h"
#include "../math/Math.h"
#include "../Debug/Debug.h"

class Sprite
{
private: // サブクラス

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 uv;
	};

	/// <summary>
	/// 定数バッファデータ構造体
	/// </summary>
	struct MaterialData {
		Vector4 color;
		Matrix4x4 mat;
	};

public:// 静的なメンバ関数

	/// <summary>
	/// 静的初期化関数
	/// </summary>
	/// <param name="device">DXGIデバイス</param>
	static void StaticInitialize(ID3D12Device* device, int windowWidth, int windowHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成関数
	/// </summary>
	/// <param name="textureHandle">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="color">色</param>
	/// <param name="anchorPoint">アンカーポイント</param>
	/// <returns>生成されたスプライト</returns>
	static Sprite* Create(
		uint32_t textureHandle, Vector2 position, Vector4 color = { 1,1,1,1 },
		Vector2 anchorPoint = { 0.0f, 0.0f }
	);

	/// <summary>
	/// DXCの初期化関数
	/// </summary>
	static void InitializeDXC();

	/// <summary>
	/// シェーダーのコンパイルを行う関数
	/// </summary>
	/// <param name="filePath">compilerするSharderファイルへのパス</param>
	/// <param name="profile">compilerに使用するprofile</param>
	/// <param name="dxcUtils">dxcUtils</param>
	/// <param name="dxcCompiler">dxcCompiler</param>
	/// <param name="includeHandler">includeHandler</param>
	/// <returns>コンパイル済みシェーダー</returns>
	static IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile);

	/// <summary>
	/// バッファリソース作成関数
	/// </summary>
	/// <param name="device">作成に使用するデバイス</param>
	/// <param name="sizeInBytes">サイズ</param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

private: // 静的なメンバ変数

	// 頂点数
	static const int kVertexNum = 4;
	// デバイス
	static ID3D12Device* sDevice_;

	// コマンドリスト
	static ID3D12GraphicsCommandList* sCommandList_;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> sRootSignature_;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> sPipelineStates_;

	// dxcUtils
	static Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;
	// dxcコンパイラ
	static Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;
	// InludeHandler
	static Microsoft::WRL::ComPtr<IDxcIncludeHandler> dxcIncludeHandler_;

	// 正射影行列
	static Matrix4x4 sMatProjection_;

public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite(uint32_t textureHandle, Vector2 position, Vector2 size,
		Vector4 color, Vector2 anchorPoint);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化出来ているか</returns>
	bool Initialize();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// スプライト中心座標のセッター
	/// </summary>
	/// /// <param name="translation">セットする座標</param>
	void SetPosition(const Vector2& translation) {
		position_ = translation;
		TransferVertices();
	}
	/// <summary>
	/// スプライト中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	Vector2 GetPosition() { return position_; }

	/// <summary>
	/// 回転角のセッター
	/// </summary>
	/// <param name="rotation"></param>
	void SetRotation(const float& rotation) {
		rotation_ = rotation;
		TransferVertices();
	}
	/// <summary>
	/// 回転角のゲッター
	/// </summary>
	/// <returns></returns>
	float GetRotation() { return rotation_; }

	/// <summary>
	/// サイズのセッター
	/// </summary>
	/// <param name="size">設定するサイズ</param>
	void SetSize(const Vector2& size) {
		size_ = size;
		TransferVertices();
	}
	/// <summary>
	/// サイズのゲッター
	/// </summary>
	/// <returns>サイズ</returns>
	Vector2 GetSize() {
		return size_;
	}

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">設定する色</param>
	void SetColor(Vector4 color) { color_ = color; }
	/// <summary>
	/// 色のゲッター
	/// </summary>
	/// <returns>色</returns>
	Vector4 GetColor() { return color_; }

	/// <summary>
	/// テクスチャハンドルの設定
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetTextureHandle(uint32_t textureHandle);

private: // メンバ変数

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファマップ
	VertexData* vertMap_ = nullptr;
	// 定数バッファマップ
	MaterialData* constMap_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// スプライト起点座標
	Vector2 position_{};
	// スプライトの幅と高さ
	Vector2 size_ = { 100.0f, 100.0f };
	// 回転角
	float rotation_ = 0.0f;
	// アンカーポイント
	Vector2 anchorPoint_ = { 0, 0 };
	// ワールド行列
	Matrix4x4 matWorld_{};

	// テクスチャ
	uint32_t textureHandle_ = 0u;
	// テクスチャ始点
	Vector2 texBase_ = { 0.0f, 0.0f };
	// テクスチャ幅と高さ
	Vector2 texSize_ = { 100.0f, 100.0f };

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc_;

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private: // メンバ関数

	/// <summary>
	/// 頂点データ転送関数
	/// </summary>
	void TransferVertices();

};

