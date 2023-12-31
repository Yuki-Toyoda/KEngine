#include "Animation.h"

void Animation::Init(const std::string& name)
{
	// アニメーション名の取得
	name_ = name;

	// キー配列クリア
	animationKeys_.clear();
}

void Animation::Update()
{
	// 配列内の全てのキーを更新する
	for (auto& keys : animationKeys_) {
		std::visit([](auto& key) { key.Update(); }, keys);
	}
}

void Animation::DisplayImGui()
{
	// アニメーション名
	std::string name = "Now SelectAnimation : " + name_;
	ImGui::Text(name.c_str());

	// メニューバーの表示
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Keys"))
		{
			// 配列のサイズが1以上の時にキー情報を描画
			if (animationKeys_.size() > 0) {
				// 現在取得中のインデックス保存用
				size_t index = 0;
				// 配列内の全てのキーを更新する
				for (auto& keys : animationKeys_) {
					std::visit([&](auto& key) {
						// キー名のアイテム表示
						if (ImGui::MenuItem(key.keysName_.c_str())) {
							imGuiSelectKey_ = (int)index;
						}
					}, keys);

					// 選択中インデックスインクリメント
					index++;
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			// キー名のアイテム表示
			if (ImGui::MenuItem("SortKeys")) {
				// 配列のサイズが1以上の時にキー情報を描画
				if (animationKeys_.size() > 0) {
					// 配列内の全てのキーをソートする
					for (auto& keys : animationKeys_) {
						std::visit([](auto& key) { key.SortKey(); }, keys);
					}
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 選択したキーの情報を表示する
	if (imGuiSelectKey_ >= animationKeys_.size() - 1) {
		imGuiSelectKey_ = (int)animationKeys_.size() - 1;
	}

	if (animationKeys_.size() > 0) {
		// 現在取得中のインデックス保存用
		size_t index = 0;
		// 配列内の全てのキーを更新する
		for (auto& keys : animationKeys_) {
			std::visit([&](auto& key) {
				// インデックスと一致した場合そのキーの情報を表示
				if (imGuiSelectKey_ == (int)index) {
					// ImGuiの表示関数
					key.DisplayImGui();
				}
				}, keys);
			// 選択中インデックスインクリメント
			index++;
		}
	}
}

void Animation::SaveAnimation()
{
	// 調整項目クラスに登録された値を外部ファイルに書き出す
	GlobalVariables::GetInstance()->SaveFile(name_);
}



