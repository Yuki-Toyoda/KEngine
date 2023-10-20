#include "SceneManager.h"
#include "user/SceneList.h"
#include "../GameObject/GameObjectManager.h"
#include "../Base/WinApp.h"

SceneManager* SceneManager::GetInstance()
{
	static SceneManager sceneManager;
	return &sceneManager;
}

void SceneManager::Initialize()
{

	// フェード演出スプライト用座標
	fadeSpritePosition_ = { 0.0f, 0.0f };
	// フェード演出スプライト用大きさ
	fadeSpriteSize_ = { (float)WinApp::kWindowWidth, (float)WinApp::kwindowHeight };
	// フェード演出スプライト用色
	fadeSpriteColor_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	// フェード演出用スプライト
	fadeSprite_.reset(Sprite::Create(TextureManager::Load("white1x1.png"), &fadeSpritePosition_, &fadeSpriteSize_, &fadeSpriteColor_));

	// フェード演出用色
	startFadeColor_ = fadeSpriteColor_; // 始端
	endFadeColor_ = { 0.0f, 0.0f, 0.0f, 0.0f }; // 終端

	// フェード演出用t
	fadeT_ = 0.0f;
	// フェード演出時間(秒)
	fadeTime_ = 1.0f;
	// フェード演出中か
	isFading_ = false;
	// フェード演出が終了しているか
	isEndFade_ = true;

	// シーン初期化
	currentScene_ = new TitleScene(); // タイトルシーン生成
	currentScene_->commonInitialize(); // 共通初期化を行う
	currentScene_->Initialize(); // 初期化を行う
}

void SceneManager::Update()
{
#ifdef _DEBUG

	

#endif // _DEBUG

	// FPSカウンターの表示
	ImGui::Begin("Control panel");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	// 現在のシーンから次のシーンへ遷移するよう指示されたら
	if (nextScene_ != nullptr) {
		GameObjectManager::GetInstance()->Initialize();

		// 現在のシーンがあるなら
		if (currentScene_) {
			// 現在のシーンを削除
			delete currentScene_;
		}

		// シーン初期化
		currentScene_ = nextScene_; // 次のシーンを取得
		nextScene_ = nullptr; // 次のシーンにNULLを代入
		currentScene_->commonInitialize(); // 取得したシーンの共通初期化を行う
		currentScene_->Initialize(); // 取得したシーンの初期化を行う
	}

	// フェードエフェクトを再生する
	if (isFading_)
		Fade();

	// 現在のシーンの更新
	currentScene_->Update();

}

void SceneManager::Draw()
{
	fadeSprite_->Draw();
}

void SceneManager::StartFadeEffect(float fadeTime, Vector4 startcolor, Vector4 endColor)
{
	// 演出中でなければ
	if (!isFading_) {
		// 始端色と終端色を取得
		startFadeColor_ = startcolor;
		endFadeColor_ = endColor;
		// 演出用t初期化
		fadeT_ = 0.0f;
		// 引数の値をメンバ変数に代入
		fadeTime_ = fadeTime;
		// 演出中にする
		isFading_ = true;
		// 演出は終了していない
		isEndFade_ = false;
	}
}

void SceneManager::Fade()
{
	if (fadeT_ <= fadeTime_) {
		// 色のイージングを行う
		fadeSpriteColor_.x = Math::EaseInOut(fadeT_, startFadeColor_.x, endFadeColor_.x, fadeTime_);
		fadeSpriteColor_.y = Math::EaseInOut(fadeT_, startFadeColor_.y, endFadeColor_.y, fadeTime_);
		fadeSpriteColor_.z = Math::EaseInOut(fadeT_, startFadeColor_.z, endFadeColor_.z, fadeTime_);
		fadeSpriteColor_.w = Math::EaseInOut(fadeT_, startFadeColor_.w, endFadeColor_.w, fadeTime_);

		// tを加算
		fadeT_ += 1.0f / 60.0f;
		// 演出中である
		isFading_ = true;
	}
	else {
		// 終端色に強制敵に遷移
		fadeSpriteColor_ = endFadeColor_;

		// 演出用tをリセット
		fadeT_ = 0.0f;
		// 演出中でない
		isFading_ = false;
		// 終了を伝える
		isEndFade_ = true;
	}
}
