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
	/// <param name="size">初期化する配列サイズ</param>
	IndexList(int size) : kSize_(size) {
		// 引数のサイズを元に使用状態配列をリサイズする
		array_.resize(kSize_, false);
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IndexList() = default;

public: // メンバ関数

	/// <summary>
	/// 空きがあるか確認する関数
	/// </summary>
	/// <returns>配列内に空きがあるか</returns>
	bool CheckEmpty() {
		// 未使用のインデックスを検索する
		for (int i = 0; i < kSize_; i++) {
			// 未使用のインデックスがある場合
			if (!array_[i]) {
				// trueを返す
				return true;
			}
		}

		// 空きがない場合falseを返す
		return false;
	}

	/// <summary>
	/// 空き番号を使用する関数
	/// </summary>
	/// <returns>インデックス</returns>
	Index UseEmpty() {
		// 未使用のインデックスを検索する
		for (int i = 0; i < kSize_; i++) {
			// 未使用のインデックスを発見した場合
			if (!array_[i]) {
				// 使用したことを記録する
				array_[i] = true;
				// インデックスクラスを返す
				return Index(
					[&](int i) {UnUse(i); }, i
				);
			}
		}

		// 未使用のインデックスがない場合配列外参照としてエラーを返す
		assert(false);
		return Index(nullptr, -1);
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
	void UnUse(int index) {
		// 指定されたインデックスの使用状態を変更
		array_[index] = false;
	}

private: // メンバ変数

	// 配列サイズ定数
	const int kSize_;

	// 使用状態記録用配列
	std::vector<bool> array_;

};

