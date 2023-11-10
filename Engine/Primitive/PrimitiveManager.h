#pragma once
#include <list>
#include "BasePrimitive.h"
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
	void Initialize();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

	/// <summary>
	/// 形状のインスタンスを生成しリストに登録する関数
	/// </summary>
	/// <typeparam name="SelectPrimitive">生成する形状</typeparam>
	/// <returns>形状インスタンス</returns>
	template <IsBasePrimitive SelectPrimitive>
	inline SelectPrimitive* CreateInstance() {
		// 形状のインスタンス
		SelectPrimitive* newPrimitive = new SelectPrimitive(commandManager_);
		newPrimitive->ResizeVertices();
		newPrimitive->ResizeIndexes();

		// 生成した形状をリストに登録
		primitives_.push_back(newPrimitive);

		// 生成した形状を返す
		return newPrimitive;
	}

private: // メンバ変数

	// コマンドマネージャー
	CommandManager* commandManager_ = nullptr;

	// 描画形状リスト
	std::list<BasePrimitive*> primitives_;

};

