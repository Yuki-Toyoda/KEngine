#pragma once
#include <list>
#include <memory>
#include "IPrimitive.h"
#include "3d/Mesh.h"

// クラスの前方宣言
class CommandManager;

/// <summary>
/// 形状マネージャ
/// </summary>
class PrimitiveManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	PrimitiveManager() = default;
	~PrimitiveManager() = default;
	PrimitiveManager(const PrimitiveManager&) = delete;
	const PrimitiveManager& operator=(const PrimitiveManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static PrimitiveManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

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
	/// 形状のインスタンスを生成しリストに登録する関数
	/// </summary>
	/// <typeparam name="SelectPrimitive">生成する形状</typeparam>
	/// <returns>形状インスタンス</returns>
	template <IsBasePrimitive SelectPrimitive>
	inline SelectPrimitive* CreateInstance() {
		// 形状のインスタンス
		std::unique_ptr<SelectPrimitive> newPrimitive = std::make_unique<SelectPrimitive>();
		newPrimitive->ResizeVertices();
		newPrimitive->ResizeIndexes();

		// インスタンス変換用のオブジェクト
		SelectPrimitive* returnPrimitive = newPrimitive.get();

		// 生成した形状をリストに登録
		primitives_.push_back(std::move(newPrimitive));

		// 生成した形状を返す
		return returnPrimitive;
	}

public: // アクセッサ等

	/// <summary>
	/// 描画リストゲッター
	/// </summary>
	/// <returns>描画リスト</returns>
	const std::list<std::unique_ptr<IPrimitive>>& GetPrimitives() const { return primitives_; }

private: // メンバ変数

	// 描画形状リスト
	std::list<std::unique_ptr<IPrimitive>> primitives_;

};

