#include "GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void GameManager::Init()
{
	// 入力取得
	input_ = Input::GetInstance();

	// アニメーションマネージャの取得
	animManager_ = AnimationManager::GetInstance();

	// タイトル演出用カメラを生成
	titleCamera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("TitleCamera", IObject::TagCamera);
	titleCamera_->transform_.translate_ = { -1.5f, 0.92f, -3.5f };
	titleCamera_->transform_.rotate_ = { -0.15f, 0.325f, 0.0f };
	titleCamera_->ppProcessor_.bloom_.SetThreshold(0.25f);
	titleCamera_->UseThisCamera(false);

	/// タイトル演出用パラメータを生成
	CreateTitleCameraParameter("Title_Idle");
	CreateTitleCameraParameter("Title_Start");

	// アニメーション生成
	titleAnim_ = AnimationManager::GetInstance()->CreateAnimation("TitleAnim", "Title_Idle");
	titleAnim_->AddAnimationKeys<Vector3>("Camera_Rotate", &titleCamera_->transform_.rotate_);
	titleAnim_->AddAnimationKeys<Vector3>("Camera_Translate", &titleCamera_->transform_.translate_);
	// ループ状態にする
	titleAnim_->isLoop_ = true;
	// この状態で再生
	titleAnim_->Play();

	/// タイトル演出用スプライトの生成
	// タイトル画面ボタン用
	AddSprite("StartButton", { 640.0f, 450.0f }, { 512.0f ,128.0f }, TextureManager::Load("Start_CR.png"));
	// アンカーポイントの設定
	sprites_["StartButton"]->anchorPoint_ = { .5f, .5f };

	// フェード演出用スプライトの追加
	AddSprite("Fade", {
		(float)KEngine::Config::Window::KWindowWidth / 2.0f,
		(float)KEngine::Config::Window::KWindowHeight / 2.0f, },
		{ (float)KEngine::Config::Window::KWindowWidth,
		(float)KEngine::Config::Window::KWindowHeight },
		TextureManager::Load("white2x2.png"));
	// アンカーポイントの設定
	sprites_["Fade"]->anchorPoint_ = { .5f, .5f };
	// 色、透明度設定
	sprites_["Fade"]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void GameManager::Update()
{
	// タイトル演出更新関数
	TitleStagingUpdate();

	// ゲームの進行状態の確認
	GameStateUpdate();

	// フェード演出中であれば
	if (isFadeStaging_) {
		// フェード演出更新
		FadeUpdate();
	}
}

void GameManager::DisplayImGui()
{
	// デバッグの有効化状態チェックボックス
	ImGui::Checkbox("Enable Debug", &enableDebug_);
	// テキスト表示
	if (enableDebug_) { // デバッグが有効であるとき
		ImGui::Text("Press 'SELECT' Enable Debug");
	}
	else { // デバッグが有効でないとき
		ImGui::Text("Press 'SELECT' Disable Debug");
	}
	ImGui::NewLine(); // 改行

	// ゲームの状態表示
	ImGui::Checkbox("Is GameStart", &isGameStart_);
	ImGui::Checkbox("Is GameEnd", &isGameEnd_);

	/// フェード演出関係
	if (ImGui::TreeNode("Fade")) {
		// 現在演出中か
		if (isFadeStaging_) {
			// 演出中ならテキスト表示
			ImGui::Text("FadeStaging!");
		}
		// フェード演出の状態を表示
		switch (fadeState_)
		{
		case GameManager::FADEIN:
			ImGui::Text("FadeState - FadeIN");
			break;
		case GameManager::FADEOUT:
			// 演出中ならテキスト表示
			ImGui::Text("FadeState - FadeOUT");
			break;
		}

		// フェード演出タイマーのImGuiを表示
		fadeTimer_.DisplayImGui("FadeTimer");

		ImGui::TreePop();
	}
}

void GameManager::StartFade(const int fadeState, const float fadeTime)
{
	// フェード演出中トリガーを取得
	isFadeStaging_ = true;
	// フェード演出状態の指定
	fadeState_ = fadeState;
	// フェード演出タイマー開始
	fadeTimer_.Start(fadeTime);
	// 現在のフェード演出スプライトの透明度を取得
	currentFadeAlpha_ = sprites_["Fade"]->color_.w;
}

void GameManager::SetFade(const float fadeAlpha)
{
	// スプライトの色を直接指定
	sprites_["Fade"]->color_.w = fadeAlpha;
	// 演出中フラグをfalseに
	isFadeStaging_ = false;
}

void GameManager::TitleStagingUpdate()
{
	// ゲームが開始されていない状態の場合
	if (!isGameStart_) {
		// タイトルアニメーションが終了していない、かつタイトルアニメーションの読み込みパラメーターが開始アニメーションでないとき
		if (!titleAnim_->isEnd_ || titleAnim_->GetReadingParameterName() != "Title_Start") {
			// Aボタンを押すと
			if (input_->InspectButton(XINPUT_GAMEPAD_A, TRIGGER) && !isGameStarting_) {

				// 強制フェードイン
				SetFade(0.0f);

				// タイトルアニメーション再生
				titleAnim_->ChangeParameter("Title_Start", true);
				// ループ無効
				titleAnim_->isLoop_ = false;
				// ゲーム開始演出中に
				isGameStarting_ = true;
			}
		}
		else { // タイトルアニメーション終了時
			// ゲーム開始状態
			isGameStart_ = true;
		}
	}

	// 開始アニメーション再生時
	if (titleAnim_->GetReadingParameterName() == "Title_Start") {
		if (titleAnim_->GetIsPlay()) {
			// ボタンスプライトの透明度を徐々に下げる
			sprites_["StartButton"]->color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInQuad(titleAnim_->GetAnimationProgress()));
		}
		else {
			// ボタンスプライトの透明度を0にする
			sprites_["StartButton"]->color_.w = 0.0f;
		}
	}
}

void GameManager::CreateTitleCameraParameter(const std::string& name)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Translate");
}

void GameManager::GameStateUpdate()
{
	// 敵、またはプレイヤーの死亡トリガーがtrueになってる場合はゲームが終了している
	if (enemy_->GetIsDead() || player_->GetIsDead()) {
		// 終了演出の開始フラグ
		if (!isEndStaging_) {
			// フェードアウト開始
			StartFade(FADEOUT, deadFadeOutStagingTime_);

			// 終了演出フラグをtrue
			isEndStaging_ = true;
		}
	}

	// 終了演出を開始していてｍフェード演出が終了している場合
	if (isEndStaging_ && !isFadeStaging_) {
		// ゲーム終了
		isGameEnd_ = true;
	}
}

void GameManager::FadeUpdate()
{
	// 目標値の設定
	float targetAlpha = 0.0f;

	// フェード状態によって目標値を決める
	switch (fadeState_)
	{
	case GameManager::FADEIN: // フェードイン
		targetAlpha = 0.0f;
		break;
	case GameManager::FADEOUT: // フェードアウト
		targetAlpha = 1.0f;
		break;
	}

	// フェードタイマーが終了していないとき
	if (!fadeTimer_.GetIsFinish()) {
		// フェード処理を実際に行う
		sprites_["Fade"]->color_.w = KLib::Lerp<float>(currentFadeAlpha_, targetAlpha, fadeTimer_.GetProgress());
	}
	else {
		// 終了時は演出中フラグのリセット、および目標値に設定しておく
		isFadeStaging_ = false;
		sprites_["Fade"]->color_.w = targetAlpha;
	}

	// フェード演出タイマーの更新
	fadeTimer_.Update();
}
