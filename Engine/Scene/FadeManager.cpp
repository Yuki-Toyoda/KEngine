#include "FadeManager.h"
#include "../Base/WinApp.h"
#include "../Resource/Texture/TextureManager.h"

void FadeManager::Init()
{
	// ウィンドウサイズの取得
	Vector2 windowSize = Vector2((float)WinApp::kWindowWidth, (float)WinApp::kwindowHeight);

	// インスタンス生成
	sprite_ = std::make_unique<Sprite>();

	// スプライト初期化
	sprite_->FadeInit(
		{ windowSize.x / 2.0f, windowSize.y / 2.0f },
		windowSize,
		TextureManager::Load("white2x2.png"));
	// 透明度は0
	sprite_->color_ = {0.0f, 0.0f, 0.0f, 0.0f};
	// アンカーポイントの設定
	sprite_->anchorPoint_ = { 0.5f, 0.5f };

	// アニメーションマネージャ取得
	animManager_ = AnimationManager::GetInstance();
	
	/// パラメータ生成
	// フェードアウト
	CreateParameter("FadeOut");
	// フェードイン
	CreateParameter("FadeIn");

	// アニメーション生成
	anim_ = animManager_->CreateFadeAnimation("FadeIn");
	// キーの追加
	anim_->AddAnimationKeys<Vector2>("Scale", &sprite_->scale_);
	anim_->AddAnimationKeys<float>("Rotate", &sprite_->rotate_);
	anim_->AddAnimationKeys<Vector2>("Translate", &sprite_->translate_);
	anim_->AddAnimationKeys<float>("Color_R", &sprite_->color_.x);
	anim_->AddAnimationKeys<float>("Color_G", &sprite_->color_.y);
	anim_->AddAnimationKeys<float>("Color_B", &sprite_->color_.z);
	anim_->AddAnimationKeys<float>("Alpha", &sprite_->color_.w);
}

void FadeManager::Update()
{
	// アニメーション更新
	anim_->Update();

	// スプライトの更新処理を呼び出す
	sprite_->Update();
}

void FadeManager::DisplayImGui()
{
	// ImGuiの開始
	ImGui::Begin("Fade");

	// スプライトのImGuiの表示
	sprite_->DisplayImGui();

	// アニメーションのImGuiの表示
	anim_->DisplayImGui();
	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("ChangeReadingParameter")) {
		if (ImGui::Button("FadeOut")) {
			anim_->ChangeParameter("FadeOut", true);
		}
		if (ImGui::Button("FadeIn")) {
			anim_->ChangeParameter("FadeIn", true);
		}
		ImGui::TreePop();
	}

	if (ImGui::Button("CompulsionFadeOut")) {
		CompulsionFadeOut();
	}
	if (ImGui::Button("CompulsionFadeIn")) {
		CompulsionFadeIn();
	}

	ImGui::End();
}

void FadeManager::CreateParameter(const std::string& name)
{
	// パラメータ生成
	animManager_->CreateAnimationParameter(name);
	// キーの生成
	animManager_->AddSelectAnimationKeys<Vector2>(name, "Scale");
	animManager_->AddSelectAnimationKeys<float>(name, "Rotate");
	animManager_->AddSelectAnimationKeys<Vector2>(name, "Translate");
	animManager_->AddSelectAnimationKeys<float>(name, "Color_R");
	animManager_->AddSelectAnimationKeys<float>(name, "Color_G");
	animManager_->AddSelectAnimationKeys<float>(name, "Color_B");
	animManager_->AddSelectAnimationKeys<float>(name, "Alpha");
}

void FadeManager::CompulsionFadeOut()
{
	// アニメーションの再生を停止
	anim_->Stop();

	// ウィンドウサイズの取得
	Vector2 windowSize = Vector2((float)WinApp::kWindowWidth, (float)WinApp::kwindowHeight);
	// 座標を中心に
	sprite_->translate_ = { windowSize.x / 2.0f, windowSize.y / 2.0f };

	// 演出用スプライトの透明度を1に
	sprite_->color_.w = 1.0f;
}

void FadeManager::CompulsionFadeIn()
{
	// アニメーションの再生を停止
	anim_->Stop();

	// ウィンドウサイズの取得
	Vector2 windowSize = Vector2((float)WinApp::kWindowWidth, (float)WinApp::kwindowHeight);
	// 座標を中心に
	sprite_->translate_ = { windowSize.x / 2.0f, windowSize.y / 2.0f };

	// 演出用スプライトの透明度を0に
	sprite_->color_.w = 0.0f;
}

void FadeManager::Play()
{
	// 再生する
	anim_->Play();
}

void FadeManager::ChangeParameter(const std::string& name, bool isChange)
{
	// アニメーションの読み込みパラメータを変更
	anim_->ChangeParameter(name, isChange);
}
