#include "AnimationManager.h"

void AnimationManager::Init()
{
	// アニメーションキー配列クリア
	animations_.clear();
}

void AnimationManager::Update()
{
	// 終了したアニメーションを削除
	animations_.remove_if([](std::unique_ptr<MyAnimation>& animation) {
		if (animation->isDestruction_) {
			return true;
		}
		return false;
		});

	// 全てのアニメーションを更新する
	for (std::unique_ptr<MyAnimation>& a : animations_) {
		a->Update();
	}
}

void AnimationManager::DisplayImGui()
{
#ifdef _DEBUG

	ImGui::Begin("AnimationParameters", nullptr, ImGuiWindowFlags_MenuBar);
	// メニューバーの表示
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File"))
		{
			// パラメーター配列に要素がある場合
			if (parameters_.size() > 0) {
				// 配列終端より超過したパラメーターを選択していた場合その範囲を制限
				if (parameters_.size() <= imGuiSelectAnimation_) {
					imGuiSelectAnimation_ = (int)parameters_.size() - 1;
				}
				// 表示中のアニメーションのキー情報保存
				if (ImGui::MenuItem("Save")) {
					// 表示中アニメーションの保存を行う
					parameters_[imGuiSelectAnimation_].SaveAnimation();
				}
				if (ImGui::MenuItem("SaveALL")) {
					// 全てのアニメーションの保存を行う
					for (MyAnimation& a : parameters_) {
						a.SaveAnimation();
					}
				}
			}
			else {
				// アニメーションを追加しるよう促す
				ImGui::Text("Need Add Animation");
			}
			ImGui::EndMenu();
		}
		// ImGuiで編集するアニメーションの切り替え
		if (ImGui::BeginMenu("Animations"))
		{
			// 全てのアニメーション分のアイテムを表示
			for (int i = 0; i < parameters_.size(); i++) {
				// 表示アニメーション切り替え
				if (ImGui::MenuItem(parameters_[i].name_.c_str())) {
					imGuiSelectAnimation_ = i;
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// パラメーター配列に要素がある場合
	if (parameters_.size() > 0) {
		// 選択中のアニメーションのImGuiを表示
		parameters_[imGuiSelectAnimation_].DisplayParameterImGui();
	}
	else {
		// アニメーションがない
		ImGui::Text("NoAnimations");
	}

	ImGui::End();

#endif // _DEBUG
}

void AnimationManager::CreateAnimationParameter(const std::string name)
{
	// 保存されているパラメーター内に同名のアニメーションが存在しない場合はパラメーターとして追加する
	for (MyAnimation& a : parameters_) {
		// 同名のパラメーターが見つかった場合
		if (a.name_ == name) {
			// 処理を抜ける
			return;
		}
	}

	// 新しいアニメーションを生成
	MyAnimation newAnim;
	// 生成したアニメーションを初期化する
	newAnim.Init(name);

	// 配列に要素を追加
	parameters_.push_back(newAnim);
}

MyAnimation* AnimationManager::CreateAnimation(const std::string& name, const std::string& parameterName)
{
	// 新しいアニメーションを生成
	std::unique_ptr<MyAnimation> newAnimation = std::make_unique<MyAnimation>();
	// 生成したアニメーションの初期化
	newAnimation->Init(name, parameterName);

	// 返還用インスタンス生成
	MyAnimation* returnAnimation = newAnimation.get();

	// 生成したアニメーションを配列に追加
	animations_.push_back(std::move(newAnimation));

	// 生成したアニメーションを返す
	return returnAnimation;
}