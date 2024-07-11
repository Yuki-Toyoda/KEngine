#include "Animation.h"

void MyAnimation::Init(const std::string& name)
{
	// アニメーション名の取得
	name_ = name;

	// キー配列クリア
	animationKeys_.clear();
}

void MyAnimation::Init(const std::string& name, const std::string& parameterName)
{
	// アニメーション名の取得
	name_ = name;
	// パラメータ名の取得
	parameterName_ = parameterName;

	// キー配列クリア
	animationKeys_.clear();
}

void MyAnimation::Update()
{
	// 一度全てのキーは終了している状態に
	isAllKeyEnd_ = true;

	// 配列内の全てのキーを更新する
	for (auto& keys : animationKeys_) {
		std::visit([](auto& key) {
			// キーを更新
			key.Update();
			}, keys);
	}

	// 配列内の全てのキーが終了しているかを確認
	for (auto& keys : animationKeys_) {
		std::visit([&](auto& key) {
			if (!key.isEnd_) {
				// この時点で全てのキーは終了していない
				isAllKeyEnd_ = false;
			}
			}, keys);

		// 全てのキーが終了していない時点でループを抜ける
		if (!isAllKeyEnd_) {
			break;
		}
	}

	// 全てのキーが終了しているなら
	if (isAllKeyEnd_) {
		// ループするのであれば
		if (isLoop_) {
			// 全てのキーを
			for (auto& keys : animationKeys_) {
				std::visit([](auto& key) {
					// 最初のフレームからキーを再生
					key.Play(0);
					}, keys);
			}
		}
		else {
			// アニメーションは終了している
			isEnd_ = true;
		}
	}

}

void MyAnimation::Play()
{
	// 全てのキーを
	for (auto& keys : animationKeys_) {
		std::visit([](auto& key) {
			// 最初のフレームからキーを再生
			key.Play(0);
			}, keys);
	}

	// アニメーションは終了していない
	isEnd_ = false;
	// 全てのキーは終了していない
	isAllKeyEnd_ = false;
}

void MyAnimation::Play(const float& trantionTime)
{
	// 全てのキーを
	for (auto& keys : animationKeys_) {
		std::visit([&trantionTime](auto& key) {
			// 最初のフレームからキーを再生
			key.Play(trantionTime);
			}, keys);
	}

	// アニメーションは終了していない
	isEnd_ = false;
	// 全てのキーは終了していない
	isAllKeyEnd_ = false;
}

void MyAnimation::Stop()
{
	// 全てのキーを
	for (auto& keys : animationKeys_) {
		std::visit([](auto& key) {
			// 再生停止
			key.isPlay_ = false;
			}, keys);
	}
}

bool MyAnimation::GetIsPlay()
{
	// 値返還用
	bool isPlay = false;

	// キー配列が１つでも再生されているのであれば再生中である
	for (auto& keys : animationKeys_) {
		std::visit([&isPlay](auto& key) {
			// １つでも再生されていればtrueを返す
			if (key.isPlay_) {
				isPlay = true;
			}

			}, keys);
	}

	// 再生中でない
	return isPlay;
}

void MyAnimation::ChangeParameter(const std::string name, bool isChange)
{
	// パラメータ名の取得
	parameterName_ = name;
	// アニメーションは終了していない
	isEnd_ = false;

	// 全キー配列の読み込みパラメータを変更する
	for (auto& keys : animationKeys_) {
		std::visit([&](auto& key) {
			// 読み込みパラメータ名変更
			key.name_ = parameterName_;
			}, keys);
	}

	// 強制遷移を行うのであればキーの最初から再生
	if (isChange) {
		// 再生中ならそれで再生
		if (GetIsPlay()) {
			Play();
		}
	}
}

void MyAnimation::ChangeParameter(const std::string name, const float& transitionTime, bool isChange)
{
	// パラメータ名の取得
	parameterName_ = name;
	// アニメーションは終了していない
	isEnd_ = false;

	// 全キー配列の読み込みパラメータを変更する
	for (auto& keys : animationKeys_) {
		std::visit([&](auto& key) {
			// 読み込みパラメータ名変更
			key.name_ = parameterName_;
			}, keys);
	}

	// 強制遷移を行うのであればキーの最初から再生
	if (isChange) {
		Play(transitionTime);
	}
}

float MyAnimation::GetAnimationProgress()
{
	// フレーム最大数
	int32_t maxFrameCount = 0;
	// 返還用変数
	float progress = 0.0f;

	for (auto& keys : animationKeys_) {
		std::visit([&maxFrameCount, &progress](auto& key) {
			// アニメーションのフレーム数が最も大きい値を探してその進捗を受け取る
			if (maxFrameCount < key.GetAnimationFrameCount()) {
				progress = key.GetKeysProgress();
				maxFrameCount = key.GetAnimationFrameCount();
			}
			}, keys);
	}

	return progress;
}

void MyAnimation::SetAnimationSpeed(const float& animSpeed)
{
	// 全キー配列の再生速度を変更する
	for (auto& keys : animationKeys_) {
		std::visit([&animSpeed](auto& key) {
			// 再生速度を変更
			key.SetAnimSpeed(animSpeed);
			}, keys);
	}
}

void MyAnimation::DisplayImGui()
{
	if (ImGui::TreeNode(name_.c_str())) {

		// 再生中ならストップを可能に
		if (GetIsPlay()) {
			// ボタンを押したら再生停止
			if (ImGui::Button("Stop")) {
				Stop();
			}
		}
		else {
			// ボタンを押したら再生開始
			if (ImGui::Button("Play")) {
				Play();
			}
		}

		// 表示テキスト
		std::string display;

		// 再生状態の表示
		if (GetIsPlay()) {
			display = "PlayState : Playing";
		}
		else {
			display = "PlayState : Ending";
		}
		ImGui::Text(display.c_str());

		// 読み込んでいるパラメータ名の表示
		display = "ReadingParameter : " + parameterName_;
		ImGui::Text(display.c_str());

		// ループ設定
		ImGui::Checkbox("isLoop", &isLoop_);

		// アニメーションの進捗
		float progress = GetAnimationProgress();
		ImGui::Text("Progress : %4.2f", progress);

		// タイマーのImGuiを表示
		ImGui::DragFloat("AnimSpeed", &imGuiAnimSpeed_, 0.05f, 0.05f, 10.0f);
		// ボタンを押すとアニメーション再生速度を指定された値でセット
		if (ImGui::Button("SetAnimSpeed")) {
			SetAnimationSpeed(imGuiAnimSpeed_);
		}

		ImGui::TreePop();
	}
}

void MyAnimation::DisplayParameterImGui()
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

void MyAnimation::SaveAnimation()
{
	// 調整項目クラスに登録された値を外部ファイルに書き出す
	GlobalVariables::GetInstance()->SaveFile(name_);
}