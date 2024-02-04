#include "TitleManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../Core/Camera.h"
#include "../Player/PlayerAnimManager.h"
#include "../../../Scene/FadeManager.h"

void TitleManager::Init()
{
	// 音再生クラスのインスタンス取得
	audio_ = Audio::GetInstance();

	// BGMロード
	bgmHandle_ = audio_->LoadWave("./Resources/Audio/BGM/title.wav");

	/// 効果音ロード
	// 決定音
	decisionSE_ = audio_->LoadWave("./Resources/Audio/SE/decision.wav");

	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// フェード演出マネージャ取得
	fadeManager_ = FadeManager::GetInstance();

	// 柵用ワールドトランスフォームの初期化
	fenceTransform_.Init();

	/// メッシュの追加
	// 地面の表示
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Plane", "Plane.obj");
	// 回転させる
	transform_.rotate_.x = (float)std::numbers::pi / 2.0f;
	transform_.scale_ = { 50.0f, 50.0f, 1.0f };
	meshes_[0]->texture_ = TextureManager::Load("./Resources", "GroundTex.png");
	meshes_[0]->material_.uvTransform_.scale_ = { transform_.scale_.x / 2.0f,  transform_.scale_.y / 2.0f, 1.0f };
	
	// 柵の表示
	AddMesh(&fenceTransform_, color_, "./Engine/Resource/Samples/Plane", "Plane.obj");
	// 座標変更
	fenceTransform_.translate_ = { 0.0f, 1.0f, 5.0f };
	// 大きさ変更
	fenceTransform_.scale_ = { 50.0f, 1.0f, 1.0f };
	// 柵のテクスチャに変更
	meshes_[1]->texture_ = TextureManager::Load("./Resources", "FenceTex.png");
	meshes_[1]->material_.uvTransform_.scale_ = { fenceTransform_.scale_.x / 2.0f,  1.0f, 1.0f };
	
	/// スプライトの追加
	// タイトル画面
	AddSprite("Title", { 0.0f, 0.0f }, { 720.0f, 360.0f }, TextureManager::Load("./Resources/UI/Title", "Title.png"));
	// アンカーポイント設定
	sprites_[0]->anchorPoint_ = { 0.5f, 0.5f };
	// ボタン
	AddSprite("Buttom", { 0.0f, 0.0f }, { 544.0f, 96.0f }, TextureManager::Load("./Resources/UI/Title", "PushA_CR.png"));
	// アンカーポイント設定
	sprites_[1]->anchorPoint_ = { 0.5f, 0.5f };

	// アニメーションマネージャ取得
	animManager_ = AnimationManager::GetInstance();

	/// パラメータ生成
	// ゲーム開始時のタイトルアニメーション
	CreateParameter("Title_Start");
	// 待機アニメーション
	CreateParameter("Title_Idle");
	// ゲーム開始アニメーション
	CreateParameter("Title_GameStart");

}

void TitleManager::Update()
{
	// 再生されていなければ再生する
	// 再生されていなければ再生する
	if (!isSceneChange_) {
		if (!audio_->IsPlaying(bgmVoiceHadle_) || bgmVoiceHadle_ == -1) {
			bgmVoiceHadle_ = audio_->PlayWave(bgmHandle_, false, 1.0f);
		}
	}

	// BGM音量を設定
	//audio_->SetVolume(bgmHandle_, FadeManager::GetInstance()->GetVolume());

	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// アニメーションが開始アニメーション
	if (anim_->GetReadingParameterName() == "Title_Start") {
		// アニメーションが終了、またはAボタンを押したら
		if (anim_->isEnd_ || 
			(joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))) {
			// ループを有効
			anim_->isLoop_ = true;
			// アニメーションを待機状態に変更
			anim_->ChangeParameter("Title_Idle", true);
		}
	}
	// アニメーションが待機状態の時
	if (anim_->GetReadingParameterName() == "Title_Idle") {
		// Aボタンを押したら
		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))) {
			// ループを無効
			anim_->isLoop_ = false;

			// 決定音再生
			audio_->PlayWave(decisionSE_);

			// アニメーションをゲーム開始状態に変更
			anim_->ChangeParameter("Title_GameStart", true);
		}
	}
	// アニメーションがゲーム開始状態の時
	if (anim_->GetReadingParameterName() == "Title_GameStart") {
		if (anim_->GetAnimationProgress() >= 0.1f) {
			// Aボタンを押したら
			if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A))) {
				// フェード演出に一回も入っていなければフェードアウトさせる
				if (!isFade_) {
					// フェードアウトさせる
					fadeManager_->ChangeParameter("FadeOut");
					fadeManager_->Play();
					// トリガーをtrue
					isFade_ = true;
				}
			}
		}
		if (anim_->GetAnimationProgress() >= 0.75f) {
			// フェード演出に一回も入っていなければフェードアウトさせる
			if (!isFade_) {
				// フェードアウトさせる
				fadeManager_->ChangeParameter("FadeOut");
				fadeManager_->Play();
				// トリガーをtrue
				isFade_ = true;
			}
		}

		// フェードマネージャが演出中ではない、かつフェード演出トリガーがtrueなら
		if (!fadeManager_->GetIsStaging() && isFade_) {
			isSceneChange_ = true;

			// bgm停止
			audio_->StopWave(bgmVoiceHadle_);
			
		}
	}
}

void TitleManager::DisplayImGui()
{
	// トランスフォームの表示
	transform_.DisplayImGui();

	// フェンスのトランスフォームの表示
	fenceTransform_.DisplayImGuiWithTreeNode("Fence");
	meshes_[1]->material_.uvTransform_.DisplayImGuiWithTreeNode("Fence_uv");

	// カメラのトランスフォームの表示
	camera_->transform_.DisplayImGuiWithTreeNode("Camera");

	// アニメーションのImGuiの表示
	anim_->DisplayImGui();
	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("Title_ChangeReadParameter")) {
		if (ImGui::Button("TitleStart")) {
			anim_->ChangeParameter("Title_Start", true);
		}
		if (ImGui::Button("TitleIdle")) {
			anim_->ChangeParameter("Title_Idle", true);
		}
		if (ImGui::Button("TitleGameStart")) {
			anim_->ChangeParameter("Title_GameStart", true);
		}
		ImGui::TreePop();
	}

	// プレイヤーモデルのトランスフォームの表示
	pam_->DisplayImGui();
}

void TitleManager::CreateParameter(const std::string& parameterName)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(parameterName);
	// キーの生成
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Camera_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Camera_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Player_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Player_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(parameterName, "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector2>(parameterName, "Title_Scale");
	animManager_->AddSelectAnimationKeys<Vector2>(parameterName, "Title_Translate");
	animManager_->AddSelectAnimationKeys<float>(parameterName, "Title_Alpha");
	animManager_->AddSelectAnimationKeys<Vector2>(parameterName, "Button_Scale");
	animManager_->AddSelectAnimationKeys<Vector2>(parameterName, "Button_Translate");
	animManager_->AddSelectAnimationKeys<float>(parameterName, "Button_Alpha");
}

void TitleManager::CreateAnimation()
{
	// アニメーション生成
	anim_ = animManager_->CreateAnimation("TitleAnim", "Title_Start");
	// キーの追加
	anim_->AddAnimationKeys<Vector3>("Camera_Rotate", &camera_->transform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Camera_Translate", &camera_->transform_.translate_);
	anim_->AddAnimationKeys<Vector3>("Player_Rotate", &pam_->transform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Player_Translate", &pam_->transform_.translate_);
	anim_->AddAnimationKeys<Vector3>("Body_Scale", &pam_->bodyTransform_.scale_);
	anim_->AddAnimationKeys<Vector3>("Body_Rotate", &pam_->bodyTransform_.rotate_);
	anim_->AddAnimationKeys<Vector3>("Body_Translate", &pam_->bodyTransform_.translate_);
	anim_->AddAnimationKeys<Vector2>("Title_Scale", &sprites_[0]->scale_);
	anim_->AddAnimationKeys<Vector2>("Title_Translate", &sprites_[0]->translate_);
	anim_->AddAnimationKeys<float>("Title_Alpha", &sprites_[0]->color_.w);
	anim_->AddAnimationKeys<Vector2>("Button_Scale", &sprites_[1]->scale_);
	anim_->AddAnimationKeys<Vector2>("Button_Translate", &sprites_[1]->translate_);
	anim_->AddAnimationKeys<float>("Button_Alpha", &sprites_[1]->color_.w);

	// パラメータ変更
	anim_->ChangeParameter("Title_Start", true);
	// アニメーション再生
	anim_->Play();
}
