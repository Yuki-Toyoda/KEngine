#pragma once
#include "Index.h"
#include <vector>
#include <cassert>
#include <string>
#include "../../../Externals/imgui/imgui.h"

/// <summary>
/// 使用中のインデックスを判別するインデックスリスト
/// </summary>
class IndexList
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	IndexList() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">配列の初期サイズ</param>
	IndexList(int size);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">配列の初期サイズ</param>
	/// <param name="offset">オフセット</param>
	IndexList(int size, int offset);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IndexList() = default;

public: // メンバ関数

	/// <summary>
	/// 空きがあるか確認する関数
	/// </summary>
	/// <returns>配列内に空きがあるか</returns>
	bool CheckEmpty();

	/// <summary>
	/// 空き番号を使用する関数
	/// </summary>
	/// <returns>インデックス</returns>
	Index UseEmpty();

	/// <summary>
	/// 配列リストの使用数ゲッター
	/// </summary>
	/// <returns>配列リストの使用数</returns>
	int GetUsedCount() {
		// リスト内の使用数カウント用
		int usedCount = 0;

		// 使用済みインデックスを検索する
		for (int i = 0; i < kSize_; i++) {
			// 使用済みのインデックスを発見した場合
			if (array_[i]) {
				// 使用数加算
				usedCount++;
			}
		}

		// 使用数を返す
		return usedCount;
	}

	/// <summary>
	/// デバッグGUI表示関数
	/// </summary>
	/// <param name="name">表示名</param>
	void DisplayImGui(const std::string& name) {
		ImGui::Begin(name.c_str());
		// 全ての配列の
		for (int i = 0; i < kSize_; i++) {
			// 配列内の使用状況を表示
			ImGui::Text("%d", static_cast<int>(array_[i]));

			// 10行ごとに改行
			if (((i + 1) % 10 == 0)) {
				continue;
			}
			// 5列目にハイフン
			else if ((i + 1) % 5 == 0) {
				ImGui::SameLine();
				ImGui::Text("-");
			}

			// 改行キャンセル
			ImGui::SameLine();
		}
		ImGui::End();

	}

private: // プライベートなメンバ関数

	/// <summary>
	/// 引数で指定されたインデックスの使用状態をfalseにする
	/// </summary>
	/// <param name="index">使用状態をfalseにするインデックス</param>
	void UnUse(int index);

private: // メンバ変数

	// 配列サイズ定数
	const int kSize_;

	// 配列内のオフセット管理用
	int offset_ = 0;

	// 使用状態記録用配列
	std::vector<bool> array_;

};
