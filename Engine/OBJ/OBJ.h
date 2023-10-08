#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <vector>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <numbers>
#include <fstream>
#include <istream>

#pragma comment(lib, "dxcompiler.lib")

#include "../Base/DirectXCommon.h"
#include "../GameObject/WorldTransform.h"
#include "../Debug/Debug.h"
#include "../Math/Math.h"
#include "../Light/LightGroup.h"

class ModelManager;

/// <summary>
/// OBJファイルの読み込みと描画を行うクラス
/// </summary>
class OBJ
{
public: // サブクラス

	/// <summary>
	/// 頂点データ構造体
	/// </summary>
	struct VertexData {
		Vector4 position;
		Vector2 uv;
		Vector3 normal;
	};

	/// <summary>
	/// 定数バッファデータ構造体
	/// </summary>
	struct ConstBufferData {
		Vector4 color;
		Matrix4x4 mat;
		Matrix4x4 world;
		int32_t enableLighting;
	};

	struct MaterialData {
		// テクスチャまでのファイルパス
		std::string textureFilePath;
	};

	/// <summary>
	/// モデルデータ構造体
	/// </summary>
	struct ModelData {
		std::vector<VertexData> vertices;
		MaterialData material;
	};

public: // 静的なメンバ関数

	/// <summary>
	/// 静的初期化関数
	/// </summary>
	/// <param name="device">DXGIデバイス</param>
	static void StaticInitialize(ID3D12Device* device);

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
	/// モデル生成関数
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	/// <param name="radius">球半径</param>
	/// <param name="subdivison">分割数</param>
	/// <param name="color">色</param>
	/// <returns>生成出来たか</returns>
	static OBJ* Create(
		WorldTransform* worldTransform, Vector4 color = { 1,1,1,1 }, const std::string& directoryPath = "./Resources", const std::string& fileName = ".obj");
	static OBJ* Create(
		Vector4 color = { 1,1,1,1 }, const std::string& directoryPath = "./Resources", const std::string& fileName = ".obj");

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

	/// <summary>
	/// 描画に使うビュープロジェクション行列のセッター
	/// </summary>
	/// <param name="ViewProjectionMatrix">ビュープロジェクション行列</param>
	static void SetViewProjection(Matrix4x4 ViewProjectionMatrix) { ViewProjectionMatrix_ = ViewProjectionMatrix; }

	/// <summary>
	/// ライト有効セッター
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="active">有効非有効</param>
	static void SetLightActive(int index, bool active) { lightGroup_->SetDirectionalLightActive(index, active); }

	/// <summary>
	/// ライト角度のセッター
	/// </summary>
	static void SetLightRotation(int index, Vector3 rotation) { lightGroup_->SetDirectionalLightDirection(index, rotation); }

	/// <summary>
	/// ライト輝度セッター
	/// </summary>
	/// <param name="intensity"></param>
	static void SetLightIntensity(int index, float intensity) { lightGroup_->SetDirectionalLightIntensity(index, intensity); }

	/// <summary>
	/// ライト色セッター
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="color">色</param>
	static void SetLightColor(int index, Vector4 color) { lightGroup_->SetDirectionalLightColor(index, color); }

private: // 静的なメンバ変数

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

	// ビュープロジェクション行列
	static Matrix4x4 ViewProjectionMatrix_;

	// ライト
	static std::unique_ptr<LightGroup> lightGroup_;

public: // パブリックなメンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	OBJ();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	OBJ(Vector4 color = { 1,1,1,1 });
	/// <summary>
	/// コンストラクタ
	/// </summary>
	OBJ(WorldTransform* transform, Vector4 color = { 1,1,1,1 });

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化出来ているか</returns>
	bool Initialize(const std::string& directoryPath, const std::string& fileName);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// ワールド座標のセッター
	/// </summary>
	/// <param name="worldTransform">ワールド座標</param>
	void SetWorldTransform(WorldTransform* worldTransform) { transform_ = worldTransform; }

	/// <summary>
	/// 色セッター
	/// </summary>
	/// <param name="color">セットする色</param>
	void SetColor(Vector4 color) { color_ = color; }

private: // メンバ変数

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 頂点バッファマップ
	VertexData* vertMap_ = nullptr;
	// 定数バッファマップ
	ConstBufferData* constMap_ = nullptr;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	ModelData myModelData_; // 構築するモデルデータ

	// ワールド座標
	WorldTransform* transform_;

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